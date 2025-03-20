
/**
 * @file virtual_controller.cpp
 * @brief Implementação do controle virtual usando ViGEm
 */

#include "virtual_controller.h"
#include "../utils/logger.h"
#include <stdexcept>

VirtualController::VirtualController() 
    : m_client(nullptr), m_target(nullptr), m_initialized(false), m_connected(false) {
    // Inicializar estrutura de relatório com valores padrão
    ZeroMemory(&m_report, sizeof(XUSB_REPORT));
    
    // Valores neutros para eixos analógicos (0 = centro)
    m_report.sThumbLX = 0;
    m_report.sThumbLY = 0;
    m_report.sThumbRX = 0;
    m_report.sThumbRY = 0;
    
    // Valores zero para gatilhos
    m_report.bLeftTrigger = 0;
    m_report.bRightTrigger = 0;
    
    // Nenhum botão pressionado
    m_report.wButtons = 0;
}

VirtualController::~VirtualController() {
    if (m_connected && m_target) {
        vigem_target_remove(m_client, m_target);
        Logger::info("Controle virtual desconectado");
    }
    
    if (m_target) {
        vigem_target_free(m_target);
    }
    
    if (m_client) {
        vigem_disconnect(m_client);
        vigem_free(m_client);
        Logger::info("Cliente ViGEm desconectado e liberado");
    }
}

bool VirtualController::initialize() {
    // Criar cliente ViGEm
    m_client = vigem_alloc();
    if (!m_client) {
        Logger::error("Falha ao alocar cliente ViGEm");
        return false;
    }
    
    // Conectar ao driver ViGEm
    const VIGEM_ERROR vigemConnectResult = vigem_connect(m_client);
    if (!VIGEM_SUCCESS(vigemConnectResult)) {
        vigem_free(m_client);
        m_client = nullptr;
        
        Logger::error("Falha ao conectar ao driver ViGEm: " + std::to_string(vigemConnectResult));
        return false;
    }
    
    Logger::info("Conectado ao driver ViGEm");
    
    // Alocar controle virtual Xbox 360
    m_target = vigem_target_x360_alloc();
    if (!m_target) {
        vigem_disconnect(m_client);
        vigem_free(m_client);
        m_client = nullptr;
        
        Logger::error("Falha ao alocar controle virtual Xbox 360");
        return false;
    }
    
    // Adicionar controle virtual ao bus
    const VIGEM_ERROR targetAddResult = vigem_target_add(m_client, m_target);
    if (!VIGEM_SUCCESS(targetAddResult)) {
        vigem_target_free(m_target);
        m_target = nullptr;
        vigem_disconnect(m_client);
        vigem_free(m_client);
        m_client = nullptr;
        
        Logger::error("Falha ao adicionar controle virtual: " + std::to_string(targetAddResult));
        return false;
    }
    
    m_connected = true;
    m_initialized = true;
    
    Logger::info("Controle virtual Xbox 360 inicializado com sucesso");
    
    // Enviar estado inicial
    submitReport();
    
    return true;
}

bool VirtualController::applyAction(const ControllerAction& action) {
    if (!m_initialized || !m_connected) {
        Logger::warning("Tentativa de aplicar ação com controle não inicializado");
        return false;
    }
    
    bool result = false;
    
    switch (action.type) {
        case ControllerAction::TYPE_BUTTON:
            result = setButton(action.data.buttonData.button, action.data.buttonData.pressed);
            break;
            
        case ControllerAction::TYPE_AXIS:
            result = setAxis(action.data.axisData.axis, action.data.axisData.value);
            break;
            
        case ControllerAction::TYPE_TRIGGER:
            result = setTrigger(action.data.triggerData.trigger, action.data.triggerData.value);
            break;
            
        case ControllerAction::TYPE_NONE:
        default:
            // Nenhuma ação a aplicar
            result = true;
            break;
    }
    
    return result;
}

bool VirtualController::setButton(XUSB_BUTTON button, bool pressed) {
    if (!m_initialized || !m_connected) {
        return false;
    }
    
    if (pressed) {
        // Ativar bit do botão
        m_report.wButtons |= button;
    } else {
        // Desativar bit do botão
        m_report.wButtons &= ~button;
    }
    
    return submitReport();
}

bool VirtualController::setAxis(int axis, SHORT value) {
    if (!m_initialized || !m_connected) {
        return false;
    }
    
    // Garantir que o valor esteja no intervalo correto
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    
    switch (axis) {
        case 0: // Analógico esquerdo - eixo X
            m_report.sThumbLX = value;
            break;
            
        case 1: // Analógico esquerdo - eixo Y
            m_report.sThumbLY = value;
            break;
            
        case 2: // Analógico direito - eixo X
            m_report.sThumbRX = value;
            break;
            
        case 3: // Analógico direito - eixo Y
            m_report.sThumbRY = value;
            break;
            
        default:
            return false;
    }
    
    return submitReport();
}

bool VirtualController::setTrigger(int trigger, BYTE value) {
    if (!m_initialized || !m_connected) {
        return false;
    }
    
    switch (trigger) {
        case 0: // Gatilho esquerdo
            m_report.bLeftTrigger = value;
            break;
            
        case 1: // Gatilho direito
            m_report.bRightTrigger = value;
            break;
            
        default:
            return false;
    }
    
    return submitReport();
}

bool VirtualController::submitReport() {
    if (!m_initialized || !m_connected) {
        return false;
    }
    
    const VIGEM_ERROR updateResult = vigem_target_x360_update(m_client, m_target, m_report);
    
    if (!VIGEM_SUCCESS(updateResult)) {
        Logger::error("Falha ao atualizar estado do controle: " + std::to_string(updateResult));
        return false;
    }
    
    return true;
}
