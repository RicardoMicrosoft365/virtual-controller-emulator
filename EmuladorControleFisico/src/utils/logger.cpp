
/**
 * @file logger.cpp
 * @brief Implementação do sistema de log
 */

#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

// Inicialização de variáveis estáticas
std::ofstream Logger::m_logFile;
LogLevel Logger::m_minLevel = LOG_INFO;
bool Logger::m_consoleOutput = true;
std::mutex Logger::m_mutex;
bool Logger::m_initialized = false;

bool Logger::init(const std::string& filename, bool consoleOutput) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        // Já inicializado, fechar arquivo atual antes de reabrir
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
    }
    
    m_logFile.open(filename, std::ios::out | std::ios::app);
    if (!m_logFile.is_open()) {
        return false;
    }
    
    m_consoleOutput = consoleOutput;
    m_initialized = true;
    
    info("Sistema de log inicializado");
    return true;
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        info("Sistema de log finalizado");
        
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
        
        m_initialized = false;
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || level < m_minLevel) {
        return;
    }
    
    std::string timestamp = getTimestamp();
    std::string levelStr = getLevelString(level);
    std::string fullMessage = timestamp + " [" + levelStr + "] " + message;
    
    // Escrever no arquivo
    if (m_logFile.is_open()) {
        m_logFile << fullMessage << std::endl;
        m_logFile.flush();
    }
    
    // Escrever no console se habilitado
    if (m_consoleOutput) {
        // Cores diferentes para cada nível (apenas no Windows)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD originalAttrs;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        originalAttrs = csbi.wAttributes;
        
        switch (level) {
            case LOG_DEBUG:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                break;
            case LOG_INFO:
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE);
                break;
            case LOG_WARNING:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
                break;
            case LOG_ERROR:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                break;
            case LOG_CRITICAL:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_RED);
                break;
        }
        
        std::cout << fullMessage << std::endl;
        
        // Restaurar cor original
        SetConsoleTextAttribute(hConsole, originalAttrs);
    }
}

void Logger::debug(const std::string& message) {
    log(LOG_DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LOG_INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LOG_WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LOG_ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LOG_CRITICAL, message);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "AVISO";
        case LOG_ERROR:
            return "ERRO";
        case LOG_CRITICAL:
            return "CRÍTICO";
        default:
            return "DESCONHECIDO";
    }
}
