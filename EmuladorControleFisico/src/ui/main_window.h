
/**
 * @file main_window.h
 * @brief Interface gráfica principal do aplicativo
 */

#pragma once

#include <Windows.h>
#include <string>
#include <atomic>
#include "../utils/config_manager.h"

/**
 * @class MainWindow
 * @brief Implementa a janela principal do aplicativo
 */
class MainWindow {
public:
    /**
     * @brief Construtor
     * @param hInstance Handle da instância do aplicativo
     * @param configManager Ponteiro para o gerenciador de configurações
     * @param emulationActive Referência para a flag de emulação ativa
     */
    MainWindow(HINSTANCE hInstance, ConfigManager* configManager, std::atomic<bool>* emulationActive);
    
    /**
     * @brief Destrutor
     */
    ~MainWindow();
    
    /**
     * @brief Executa o loop de mensagens da janela
     * @param nCmdShow Flag de exibição inicial
     * @return Código de saída do aplicativo
     */
    int run(int nCmdShow);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    ConfigManager* m_configManager;
    std::atomic<bool>* m_emulationActive;
    
    // Controles da interface
    HWND m_statusLabel;
    HWND m_sensitivitySlider;
    HWND m_sensitivityValue;
    HWND m_deadzoneSlider;
    HWND m_deadzoneValue;
    
    // Valores de configuração
    float m_mouseSensitivity;
    int m_deadzone;
    
    /**
     * @brief Inicializa a janela
     * @return true se inicializada com sucesso, false caso contrário
     */
    bool initWindow();
    
    /**
     * @brief Cria os controles da interface
     */
    void createControls();
    
    /**
     * @brief Atualiza a interface com base nas configurações atuais
     */
    void updateUI();
    
    /**
     * @brief Salva as configurações atuais
     */
    void saveSettings();
    
    /**
     * @brief Manipulador de mensagens da janela
     * @param hWnd Handle da janela
     * @param message Mensagem do Windows
     * @param wParam Parâmetro wParam
     * @param lParam Parâmetro lParam
     * @return Resultado do processamento da mensagem
     */
    static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    /**
     * @brief Processa mensagem de comando (botões, menus, etc)
     * @param wParam Parâmetro wParam da mensagem
     * @param lParam Parâmetro lParam da mensagem
     */
    void processCommand(WPARAM wParam, LPARAM lParam);
    
    /**
     * @brief Processa mensagem de notificação (controles)
     * @param wParam Parâmetro wParam da mensagem
     * @param lParam Parâmetro lParam da mensagem
     */
    void processNotify(WPARAM wParam, LPARAM lParam);
    
    /**
     * @brief Processa mensagem de timer
     * @param timerId ID do timer
     */
    void processTimer(UINT_PTR timerId);
};
