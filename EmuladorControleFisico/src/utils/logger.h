
/**
 * @file logger.h
 * @brief Sistema de log para o aplicativo
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * @enum LogLevel
 * @brief Níveis de severidade para mensagens de log
 */
enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

/**
 * @class Logger
 * @brief Classe estática para registro de logs
 */
class Logger {
public:
    /**
     * @brief Inicializa o sistema de log
     * @param filename Nome do arquivo de log
     * @param consoleOutput Se true, exibe logs no console também
     * @return true se inicializado com sucesso, false caso contrário
     */
    static bool init(const std::string& filename, bool consoleOutput = true);
    
    /**
     * @brief Finaliza o sistema de log
     */
    static void shutdown();
    
    /**
     * @brief Define o nível mínimo de log
     * @param level Nível mínimo para exibição
     */
    static void setLogLevel(LogLevel level);
    
    /**
     * @brief Registra uma mensagem de log
     * @param level Nível de severidade
     * @param message Mensagem a ser registrada
     */
    static void log(LogLevel level, const std::string& message);
    
    /**
     * @brief Registra mensagem de debug
     * @param message Mensagem a ser registrada
     */
    static void debug(const std::string& message);
    
    /**
     * @brief Registra mensagem informativa
     * @param message Mensagem a ser registrada
     */
    static void info(const std::string& message);
    
    /**
     * @brief Registra aviso
     * @param message Mensagem a ser registrada
     */
    static void warning(const std::string& message);
    
    /**
     * @brief Registra erro
     * @param message Mensagem a ser registrada
     */
    static void error(const std::string& message);
    
    /**
     * @brief Registra erro crítico
     * @param message Mensagem a ser registrada
     */
    static void critical(const std::string& message);

private:
    static std::ofstream m_logFile;
    static LogLevel m_minLevel;
    static bool m_consoleOutput;
    static std::mutex m_mutex;
    static bool m_initialized;
    
    /**
     * @brief Obtém string representando o timestamp atual
     * @return String formatada com data e hora
     */
    static std::string getTimestamp();
    
    /**
     * @brief Obtém string representando o nível de log
     * @param level Nível de log
     * @return String representando o nível
     */
    static std::string getLevelString(LogLevel level);
};
