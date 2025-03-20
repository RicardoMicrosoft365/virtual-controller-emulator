
/**
 * @file main.cpp
 * @brief Ponto de entrada principal do emulador de controle virtual.
 * @author [Seu Nome]
 * @date [Data]
 */

#include <Windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include "core/interception_manager.h"
#include "core/virtual_controller.h"
#include "core/event_mapper.h"
#include "ui/main_window.h"
#include "utils/config_manager.h"
#include "utils/logger.h"

// Variável global para controlar o estado de ativação
std::atomic<bool> g_emulationActive(false);

/**
 * @brief Thread que processa os eventos de entrada e emula o controle virtual
 * @param interceptManager Gerenciador de interceptação de eventos
 * @param virtualController Controlador virtual
 * @param eventMapper Mapeador de eventos
 */
void processingThread(InterceptionManager* interceptManager, 
                      VirtualController* virtualController,
                      EventMapper* eventMapper) {
    Logger::info("Thread de processamento iniciada");

    while (true) {
        // Verificar se a emulação está ativa
        if (!g_emulationActive) {
            Sleep(100); // Reduzir uso de CPU quando inativo
            continue;
        }

        // Obter evento de entrada do Interception
        InputEvent event = interceptManager->waitForEvent(100); // timeout de 100ms
        
        if (event.type != InputEvent::TYPE_NONE) {
            // Mapear evento para ação do controle
            ControllerAction action = eventMapper->mapEvent(event);
            
            // Aplicar ação ao controle virtual
            if (action.type != ControllerAction::TYPE_NONE) {
                virtualController->applyAction(action);
            }
            
            // Decidir se o evento original deve continuar para o sistema
            if (eventMapper->shouldPassThrough(event)) {
                interceptManager->passEventThrough(event);
            }
        }
    }
}

/**
 * @brief Função que monitora a tecla F8 para ativar/desativar a emulação
 */
void toggleHotkeyMonitor() {
    Logger::info("Monitor de hotkey iniciado");
    
    while (true) {
        // Verificar se F8 foi pressionado (código 119)
        if ((GetAsyncKeyState(VK_F8) & 0x8000) != 0) {
            g_emulationActive = !g_emulationActive;
            Logger::info(g_emulationActive ? "Emulação ativada" : "Emulação desativada");
            
            // Esperar um pouco para evitar múltiplas ativações
            Sleep(200);
        }
        Sleep(50); // Verificar periodicamente
    }
}

/**
 * @brief Função principal do programa
 * @param hInstance Handle da instância do aplicativo
 * @param hPrevInstance Handle da instância anterior (sempre NULL)
 * @param lpCmdLine Linha de comando
 * @param nCmdShow Flag de exibição
 * @return Código de saída
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicializar sistema de log
    Logger::init("emulador_controle.log");
    Logger::info("Aplicação iniciada");
    
    try {
        // Carregar configurações
        ConfigManager configManager("config.json");
        Logger::info("Configurações carregadas");
        
        // Inicializar gerenciador de interceptação
        InterceptionManager interceptManager;
        if (!interceptManager.initialize()) {
            MessageBoxA(NULL, "Falha ao inicializar o driver Interception.\nVerifique se o driver está instalado e se o aplicativo está sendo executado como administrador.", 
                      "Erro de Inicialização", MB_ICONERROR);
            return 1;
        }
        Logger::info("Interception inicializado com sucesso");
        
        // Configurar filtros para capturar eventos de teclado e mouse
        interceptManager.setKeyboardFilter(INTERCEPTION_FILTER_KEY_ALL);
        interceptManager.setMouseFilter(INTERCEPTION_FILTER_MOUSE_ALL);
        
        // Inicializar controle virtual
        VirtualController virtualController;
        if (!virtualController.initialize()) {
            MessageBoxA(NULL, "Falha ao inicializar o controle virtual.\nVerifique se o driver ViGEm está instalado corretamente.", 
                      "Erro de Inicialização", MB_ICONERROR);
            return 1;
        }
        Logger::info("Controle virtual inicializado com sucesso");
        
        // Inicializar mapeador de eventos
        EventMapper eventMapper(&configManager);
        Logger::info("Mapeador de eventos inicializado");
        
        // Iniciar thread de processamento de eventos
        std::thread procThread(processingThread, &interceptManager, &virtualController, &eventMapper);
        procThread.detach(); // Desacoplar thread
        
        // Iniciar thread para monitorar a tecla de ativação (F8)
        std::thread hotkeyThread(toggleHotkeyMonitor);
        hotkeyThread.detach(); // Desacoplar thread
        
        // Inicializar e executar a interface gráfica
        MainWindow mainWindow(hInstance, &configManager, &g_emulationActive);
        return mainWindow.run(nCmdShow);
        
    } catch (const std::exception& e) {
        std::string errorMsg = "Erro inesperado: ";
        errorMsg += e.what();
        Logger::error(errorMsg);
        MessageBoxA(NULL, errorMsg.c_str(), "Erro Fatal", MB_ICONERROR);
        return 1;
    }
}
