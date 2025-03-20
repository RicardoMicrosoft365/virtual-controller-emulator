
/**
 * @file config_manager.h
 * @brief Gerencia configurações do aplicativo em formato JSON
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class ConfigManager
 * @brief Gerencia leitura/escrita de configurações em arquivos JSON
 */
class ConfigManager {
public:
    /**
     * @brief Construtor
     * @param filename Nome do arquivo de configuração
     */
    ConfigManager(const std::string& filename);
    
    /**
     * @brief Destrutor
     */
    ~ConfigManager();
    
    /**
     * @brief Carrega configurações do arquivo
     * @return true se carregado com sucesso, false caso contrário
     */
    bool loadConfig();
    
    /**
     * @brief Salva configurações no arquivo
     * @return true se salvo com sucesso, false caso contrário
     */
    bool saveConfig();
    
    /**
     * @brief Verifica se uma configuração existe
     * @param key Chave da configuração
     * @return true se a configuração existir, false caso contrário
     */
    bool hasConfig(const std::string& key) const;
    
    /**
     * @brief Obtém valor de string
     * @param key Chave da configuração
     * @param defaultValue Valor padrão caso a chave não exista
     * @return Valor da configuração ou valor padrão
     */
    std::string getStringValue(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * @brief Obtém valor inteiro
     * @param key Chave da configuração
     * @param defaultValue Valor padrão caso a chave não exista
     * @return Valor da configuração ou valor padrão
     */
    int getIntValue(const std::string& key, int defaultValue = 0) const;
    
    /**
     * @brief Obtém valor booleano
     * @param key Chave da configuração
     * @param defaultValue Valor padrão caso a chave não exista
     * @return Valor da configuração ou valor padrão
     */
    bool getBoolValue(const std::string& key, bool defaultValue = false) const;
    
    /**
     * @brief Obtém valor de ponto flutuante
     * @param key Chave da configuração
     * @param defaultValue Valor padrão caso a chave não exista
     * @return Valor da configuração ou valor padrão
     */
    float getFloatValue(const std::string& key, float defaultValue = 0.0f) const;
    
    /**
     * @brief Define valor de string
     * @param key Chave da configuração
     * @param value Valor a ser definido
     */
    void setStringValue(const std::string& key, const std::string& value);
    
    /**
     * @brief Define valor inteiro
     * @param key Chave da configuração
     * @param value Valor a ser definido
     */
    void setIntValue(const std::string& key, int value);
    
    /**
     * @brief Define valor booleano
     * @param key Chave da configuração
     * @param value Valor a ser definido
     */
    void setBoolValue(const std::string& key, bool value);
    
    /**
     * @brief Define valor de ponto flutuante
     * @param key Chave da configuração
     * @param value Valor a ser definido
     */
    void setFloatValue(const std::string& key, float value);
    
    /**
     * @brief Obtém todas as chaves de configuração
     * @return Vetor com todas as chaves
     */
    std::vector<std::string> getAllKeys() const;

private:
    std::string m_filename;
    std::unordered_map<std::string, std::string> m_stringValues;
    std::unordered_map<std::string, int> m_intValues;
    std::unordered_map<std::string, bool> m_boolValues;
    std::unordered_map<std::string, float> m_floatValues;
    
    /**
     * @brief Analisa um valor JSON e o armazena no mapa apropriado
     * @param key Chave da configuração
     * @param valueStr String contendo o valor JSON
     */
    void parseJsonValue(const std::string& key, const std::string& valueStr);
    
    /**
     * @brief Limpa todas as configurações carregadas
     */
    void clearValues();
};
