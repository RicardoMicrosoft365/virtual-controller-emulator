
/**
 * @file main_window.cpp
 * @brief Implementação da janela principal do aplicativo
 */

#include "main_window.h"
#include "../utils/logger.h"
#include <commctrl.h>
#include <string>
#include <sstream>

// Identificadores de recursos
#define IDC_STATUS_LABEL 101
#define IDC_SENSITIVITY_LABEL 102
#define IDC_SENSITIVITY_SLIDER 103
#define IDC_SENSITIVITY_VALUE 104
#define IDC_DEADZONE_LABEL 105
#define IDC_DEADZONE_SLIDER 106
#define IDC_DEADZONE_VALUE 107
#define IDC_SAVE_BUTTON 108
#define IDC_TOGGLE_BUTTON 109

// Timer para atualização da interface
#define TIMER_UPDATE_UI 1001

// Classe da janela
const char* const WINDOW_CLASS_NAME = "EmuladorControleWindowClass";

// Armazena ponteiro para a instância atual para uso no procedimento da janela
MainWindow* g_mainWindow = nullptr;

MainWindow::MainWindow(HINSTANCE hInstance, ConfigManager* configManager, std::atomic<bool>* emulationActive)
    : m_hInstance(hInstance), m_hWnd(NULL), m_configManager(configManager), 
      m_emulationActive(emulationActive), m_mouseSensitivity(1.0f), m_deadzone(3200) {
    
    // Carregar configurações
    if (m_configManager) {
        m_mouseSensitivity = m_configManager->getFloatValue("mouse_sensitivity", 1.0f);
        m_deadzone = m_configManager->getIntValue("analog_deadzone", 3200);
    }
    
    // Armazenar ponteiro para uso no procedimento da janela
    g_mainWindow = this;
}

MainWindow::~MainWindow() {
    // Limpar referência global
    if (g_mainWindow == this) {
        g_mainWindow = nullptr;
    }
}

int MainWindow::run(int nCmdShow) {
    // Inicializar controles comuns do Windows
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);
    
    // Inicializar janela
    if (!initWindow()) {
        return 1;
    }
    
    // Mostrar janela
    ShowWindow(m_hWnd, nCmdShow);
    UpdateWindow(m_hWnd);
    
    // Criar timer para atualização da interface
    SetTimer(m_hWnd, TIMER_UPDATE_UI, 100, NULL);
    
    // Loop de mensagens
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}

bool MainWindow::initWindow() {
    // Registrar classe da janela
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MainWindow::windowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = WINDOW_CLASS_NAME;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wcex)) {
        Logger::error("Falha ao registrar classe da janela");
        return false;
    }
    
    // Criar janela
    m_hWnd = CreateWindow(
        WINDOW_CLASS_NAME,
        "Emulador de Controle Virtual",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 400,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );
    
    if (!m_hWnd) {
        Logger::error("Falha ao criar janela principal");
        return false;
    }
    
    // Criar controles da interface
    createControls();
    
    return true;
}

void MainWindow::createControls() {
    // Definir fonte para os controles
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    
    // Criar label de status
    m_statusLabel = CreateWindow(
        "STATIC",
        "Status: Emulação Desativada (Pressione F8 para ativar)",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 20, 460, 30,
        m_hWnd,
        (HMENU)IDC_STATUS_LABEL,
        m_hInstance,
        NULL
    );
    SendMessage(m_statusLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // Criar controles para sensibilidade do mouse
    CreateWindow(
        "STATIC",
        "Sensibilidade do Mouse:",
        WS_CHILD | WS_VISIBLE,
        10, 70, 150, 20,
        m_hWnd,
        (HMENU)IDC_SENSITIVITY_LABEL,
        m_hInstance,
        NULL
    );
    
    m_sensitivitySlider = CreateWindow(
        TRACKBAR_CLASS,
        "",
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS,
        160, 70, 200, 30,
        m_hWnd,
        (HMENU)IDC_SENSITIVITY_SLIDER,
        m_hInstance,
        NULL
    );
    
    // Configurar slider de sensibilidade (0.1 a 5.0, multiplicado por 10 para valores inteiros)
    SendMessage(m_sensitivitySlider, TBM_SETRANGE, TRUE, MAKELPARAM(1, 50));
    SendMessage(m_sensitivitySlider, TBM_SETPOS, TRUE, (LPARAM)(m_mouseSensitivity * 10));
    
    m_sensitivityValue = CreateWindow(
        "STATIC",
        "1.0",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        370, 70, 50, 20,
        m_hWnd,
        (HMENU)IDC_SENSITIVITY_VALUE,
        m_hInstance,
        NULL
    );
    SendMessage(m_sensitivityValue, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // Criar controles para dead zone
    CreateWindow(
        "STATIC",
        "Dead Zone:",
        WS_CHILD | WS_VISIBLE,
        10, 120, 150, 20,
        m_hWnd,
        (HMENU)IDC_DEADZONE_LABEL,
        m_hInstance,
        NULL
    );
    
    m_deadzoneSlider = CreateWindow(
        TRACKBAR_CLASS,
        "",
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS,
        160, 120, 200, 30,
        m_hWnd,
        (HMENU)IDC_DEADZONE_SLIDER,
        m_hInstance,
        NULL
    );
    
    // Configurar slider de dead zone (0 a 10000)
    SendMessage(m_deadzoneSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
    SendMessage(m_deadzoneSlider, TBM_SETPOS, TRUE, (LPARAM)(m_deadzone / 100));
    
    m_deadzoneValue = CreateWindow(
        "STATIC",
        "3200",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        370, 120, 50, 20,
        m_hWnd,
        (HMENU)IDC_DEADZONE_VALUE,
        m_hInstance,
        NULL
    );
    SendMessage(m_deadzoneValue, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // Botão para salvar configurações
    HWND saveButton = CreateWindow(
        "BUTTON",
        "Salvar Configurações",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        150, 180, 180, 30,
        m_hWnd,
        (HMENU)IDC_SAVE_BUTTON,
        m_hInstance,
        NULL
    );
    SendMessage(saveButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // Botão para ativar/desativar emulação
    HWND toggleButton = CreateWindow(
        "BUTTON",
        "Ativar Emulação",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        150, 230, 180, 30,
        m_hWnd,
        (HMENU)IDC_TOGGLE_BUTTON,
        m_hInstance,
        NULL
    );
    SendMessage(toggleButton, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // Atualizar valores iniciais
    updateUI();
}

void MainWindow::updateUI() {
    // Atualizar label de sensibilidade
    std::stringstream ss;
    ss << m_mouseSensitivity;
    SetWindowText(m_sensitivityValue, ss.str().c_str());
    
    // Atualizar label de dead zone
    ss.str("");
    ss << m_deadzone;
    SetWindowText(m_deadzoneValue, ss.str().c_str());
    
    // Atualizar label de status
    std::string statusText = "Status: ";
    if (*m_emulationActive) {
        statusText += "Emulação Ativada (Pressione F8 para desativar)";
    } else {
        statusText += "Emulação Desativada (Pressione F8 para ativar)";
    }
    SetWindowText(m_statusLabel, statusText.c_str());
    
    // Atualizar texto do botão de toggle
    SetWindowText(GetDlgItem(m_hWnd, IDC_TOGGLE_BUTTON), 
                 (*m_emulationActive) ? "Desativar Emulação" : "Ativar Emulação");
}

void MainWindow::saveSettings() {
    // Salvar configurações atuais
    if (m_configManager) {
        m_configManager->setFloatValue("mouse_sensitivity", m_mouseSensitivity);
        m_configManager->setIntValue("analog_deadzone", m_deadzone);
        m_configManager->saveConfig();
        
        MessageBox(m_hWnd, "Configurações salvas com sucesso!", "Salvar Configurações", MB_OK | MB_ICONINFORMATION);
    }
}

LRESULT CALLBACK MainWindow::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Usar ponteiro para instância atual
    if (g_mainWindow) {
        switch (message) {
            case WM_COMMAND:
                g_mainWindow->processCommand(wParam, lParam);
                return 0;
                
            case WM_HSCROLL:
                if ((HWND)lParam == g_mainWindow->m_sensitivitySlider) {
                    // Atualizar valor de sensibilidade
                    int pos = (int)SendMessage(g_mainWindow->m_sensitivitySlider, TBM_GETPOS, 0, 0);
                    g_mainWindow->m_mouseSensitivity = pos / 10.0f;
                    g_mainWindow->updateUI();
                }
                else if ((HWND)lParam == g_mainWindow->m_deadzoneSlider) {
                    // Atualizar valor de dead zone
                    int pos = (int)SendMessage(g_mainWindow->m_deadzoneSlider, TBM_GETPOS, 0, 0);
                    g_mainWindow->m_deadzone = pos * 100;
                    g_mainWindow->updateUI();
                }
                return 0;
                
            case WM_TIMER:
                g_mainWindow->processTimer((UINT_PTR)wParam);
                return 0;
                
            case WM_CLOSE:
                // Salvar configurações ao fechar
                g_mainWindow->saveSettings();
                DestroyWindow(hWnd);
                return 0;
                
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void MainWindow::processCommand(WPARAM wParam, LPARAM lParam) {
    int controlId = LOWORD(wParam);
    int notificationCode = HIWORD(wParam);
    
    if (controlId == IDC_SAVE_BUTTON && notificationCode == BN_CLICKED) {
        // Botão de salvar pressionado
        saveSettings();
    }
    else if (controlId == IDC_TOGGLE_BUTTON && notificationCode == BN_CLICKED) {
        // Botão de toggle pressionado
        *m_emulationActive = !(*m_emulationActive);
        updateUI();
    }
}

void MainWindow::processNotify(WPARAM wParam, LPARAM lParam) {
    // Processar notificações de controles
}

void MainWindow::processTimer(UINT_PTR timerId) {
    if (timerId == TIMER_UPDATE_UI) {
        // Atualizar interface a cada tick do timer
        updateUI();
    }
}
