
/**
 * @file config_manager.cpp
 * @brief Implementação do gerenciador de configurações
 */

#include "config_manager.h"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>

// Implementação simplificada de parser JSON
// Em um projeto real, considere usar uma biblioteca como nlohmann/json

ConfigManager::ConfigManager(const std::string& filename)
    : m_filename(filename) {
    // Tentar carregar arquivo existente
    loadConfig();
}

ConfigManager::~ConfigManager() {
    // Salvar configurações ao destruir
    saveConfig();
}

bool ConfigManager::loadConfig() {
    // Limpar valores atuais
    clearValues();
    
    // Verificar se o arquivo existe
    if (!std::filesystem::exists(m_filename)) {
        Logger::info("Arquivo de configuração não encontrado: " + m_filename);
        return false;
    }
    
    try {
        std::ifstream file(m_filename);
        if (!file.is_open()) {
            Logger::error("Não foi possível abrir o arquivo de configuração: " + m_filename);
            return false;
        }
        
        std::string line;
        bool inObject = false;
        std::string currentKey;
        
        while (std::getline(file, line)) {
            // Remover espaços em branco
            line.erase(std::remove_if(line.begin(), line.end(), 
                [](unsigned char c) { return std::isspace(c); }), line.end());
            
            if (line.empty() || line[0] == '#') {
                continue; // Ignorar linhas vazias e comentários
            }
            
            if (!inObject && line[0] == '{') {
                inObject = true;
                continue;
            }
            
            if (inObject && line[0] == '}') {
                break;
            }
            
            // Procurar por par chave-valor
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                // Extrair chave
                std::string key = line.substr(0, colonPos);
                
                // Remover aspas da chave
                key.erase(std::remove(key.begin(), key.end(), '"'), key.end());
                key.erase(std::remove(key.begin(), key.end(), '\''), key.end());
                
                // Extrair valor
                std::string value = line.substr(colonPos + 1);
                
                // Remover vírgula do final se existir
                if (!value.empty() && value.back() == ',') {
                    value.pop_back();
                }
                
                // Armazenar par chave-valor
                parseJsonValue(key, value);
            }
        }
        
        file.close();
        Logger::info("Configurações carregadas com sucesso de " + m_filename);
        return true;
    }
    catch (const std::exception& e) {
        Logger::error("Erro ao carregar configurações: " + std::string(e.what()));
        return false;
    }
}

bool ConfigManager::saveConfig() {
    try {
        std::ofstream file(m_filename);
        if (!file.is_open()) {
            Logger::error("Não foi possível abrir o arquivo para escrita: " + m_filename);
            return false;
        }
        
        file << "{\n";
        
        // Escrever valores de string
        for (const auto& pair : m_stringValues) {
            file << "    \"" << pair.first << "\": \"" << pair.second << "\",\n";
        }
        
        // Escrever valores inteiros
        for (const auto& pair : m_intValues) {
            file << "    \"" << pair.first << "\": " << pair.second << ",\n";
        }
        
        // Escrever valores booleanos
        for (const auto& pair : m_boolValues) {
            file << "    \"" << pair.first << "\": " << (pair.second ? "true" : "false") << ",\n";
        }
        
        // Escrever valores de ponto flutuante
        for (const auto& pair : m_floatValues) {
            file << "    \"" << pair.first << "\": " << pair.second << ",\n";
        }
        
        // Remover a última vírgula se necessário
        long pos = file.tellp();
        if (pos > 3) { // "{\\n" tem tamanho 3
            file.seekp(pos - 2);
            file << "\n";
        }
        
        file << "}\n";
        file.close();
        
        Logger::info("Configurações salvas com sucesso em " + m_filename);
        return true;
    }
    catch (const std::exception& e) {
        Logger::error("Erro ao salvar configurações: " + std::string(e.what()));
        return false;
    }
}

void ConfigManager::parseJsonValue(const std::string& key, const std::string& valueStr) {
    std::string value = valueStr;
    
    // Remover espaços em branco
    value.erase(std::remove_if(value.begin(), value.end(), 
        [](unsigned char c) { return std::isspace(c); }), value.end());
    
    if (value.empty()) {
        return;
    }
    
    // Determinar o tipo do valor
    if (value[0] == '"' || value[0] == '\'') {
        // Valor de string
        value.erase(0, 1);
        if (!value.empty() && (value.back() == '"' || value.back() == '\'')) {
            value.pop_back();
        }
        m_stringValues[key] = value;
    }
    else if (value == "true" || value == "false") {
        // Valor booleano
        m_boolValues[key] = (value == "true");
    }
    else if (value.find('.') != std::string::npos) {
        // Valor de ponto flutuante
        try {
            m_floatValues[key] = std::stof(value);
        }
        catch (...) {
            Logger::warning("Valor inválido para float: " + key + " = " + value);
        }
    }
    else {
        // Valor inteiro
        try {
            m_intValues[key] = std::stoi(value);
        }
        catch (...) {
            Logger::warning("Valor inválido para int: " + key + " = " + value);
        }
    }
}

void ConfigManager::clearValues() {
    m_stringValues.clear();
    m_intValues.clear();
    m_boolValues.clear();
    m_floatValues.clear();
}

bool ConfigManager::hasConfig(const std::string& key) const {
    return m_stringValues.count(key) > 0 || 
           m_intValues.count(key) > 0 || 
           m_boolValues.count(key) > 0 || 
           m_floatValues.count(key) > 0;
}

std::string ConfigManager::getStringValue(const std::string& key, const std::string& defaultValue) const {
    auto it = m_stringValues.find(key);
    return (it != m_stringValues.end()) ? it->second : defaultValue;
}

int ConfigManager::getIntValue(const std::string& key, int defaultValue) const {
    auto it = m_intValues.find(key);
    return (it != m_intValues.end()) ? it->second : defaultValue;
}

bool ConfigManager::getBoolValue(const std::string& key, bool defaultValue) const {
    auto it = m_boolValues.find(key);
    return (it != m_boolValues.end()) ? it->second : defaultValue;
}

float ConfigManager::getFloatValue(const std::string& key, float defaultValue) const {
    auto it = m_floatValues.find(key);
    return (it != m_floatValues.end()) ? it->second : defaultValue;
}

void ConfigManager::setStringValue(const std::string& key, const std::string& value) {
    m_stringValues[key] = value;
}

void ConfigManager::setIntValue(const std::string& key, int value) {
    m_intValues[key] = value;
}

void ConfigManager::setBoolValue(const std::string& key, bool value) {
    m_boolValues[key] = value;
}

void ConfigManager::setFloatValue(const std::string& key, float value) {
    m_floatValues[key] = value;
}

std::vector<std::string> ConfigManager::getAllKeys() const {
    std::vector<std::string> keys;
    
    for (const auto& pair : m_stringValues) {
        keys.push_back(pair.first);
    }
    
    for (const auto& pair : m_intValues) {
        keys.push_back(pair.first);
    }
    
    for (const auto& pair : m_boolValues) {
        keys.push_back(pair.first);
    }
    
    for (const auto& pair : m_floatValues) {
        keys.push_back(pair.first);
    }
    
    // Remover duplicatas (caso alguma chave tenha sido redefinida com tipo diferente)
    std::sort(keys.begin(), keys.end());
    keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
    
    return keys;
}
