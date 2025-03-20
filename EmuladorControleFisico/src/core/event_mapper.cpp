
/**
 * @file event_mapper.cpp
 * @brief Implementação do mapeador de eventos
 */

#include "event_mapper.h"
#include "../utils/logger.h"
#include <algorithm>
#include <cmath>

EventMapper::EventMapper(ConfigManager* configManager)
    : m_configManager(configManager), m_mouseSensitivity(1.0f), m_deadZone(3200) {
    
    m_lastMouseMoveTime = std::chrono::steady_clock::now();
    
    // Carregar mapeamentos padrão
    // Estes serão substituídos se existirem configurações salvas
    setupDefaultMappings();
    
    // Tentar carregar mapeamentos da configuração
    loadMappingsFromConfig();
}

void EventMapper::setupDefaultMappings() {
    // Mapeamentos padrão de teclas para botões
    m_keyMappings = {
        KeyMapping(DIK_SPACE, XUSB_GAMEPAD_A),
        KeyMapping(DIK_LCONTROL, XUSB_GAMEPAD_B),
        KeyMapping(DIK_E, XUSB_GAMEPAD_X),
        KeyMapping(DIK_R, XUSB_GAMEPAD_Y),
        KeyMapping(DIK_Q, XUSB_GAMEPAD_LEFT_SHOULDER),
        KeyMapping(DIK_F, XUSB_GAMEPAD_RIGHT_SHOULDER),
        KeyMapping(DIK_TAB, XUSB_GAMEPAD_BACK),
        KeyMapping(DIK_ESCAPE, XUSB_GAMEPAD_START),
        KeyMapping(DIK_UP, XUSB_GAMEPAD_DPAD_UP),
        KeyMapping(DIK_DOWN, XUSB_GAMEPAD_DPAD_DOWN),
        KeyMapping(DIK_LEFT, XUSB_GAMEPAD_DPAD_LEFT),
        KeyMapping(DIK_RIGHT, XUSB_GAMEPAD_DPAD_RIGHT)
    };
    
    // Mapeamentos padrão de teclas para eixos analógicos
    m_axisMappings = {
        AxisMapping(DIK_A, DIK_D, 0), // Eixo X do analógico esquerdo (A=esquerda, D=direita)
        AxisMapping(DIK_W, DIK_S, 1)  // Eixo Y do analógico esquerdo (W=cima, S=baixo)
    };
    
    // Mapeamentos padrão de teclas para gatilhos
    m_triggerMappings = {
        TriggerMapping(DIK_Z, 0), // Gatilho esquerdo (Z)
        TriggerMapping(DIK_C, 1)  // Gatilho direito (C)
    };
    
    // Mapeamento padrão do mouse para o analógico direito
    m_mouseMappings = {
        MouseAxisMapping(0, 2, 1.0f, false), // Mouse X -> Analógico Direito X
        MouseAxisMapping(1, 3, 1.0f, true)   // Mouse Y -> Analógico Direito Y (invertido)
    };
    
    Logger::info("Mapeamentos padrão configurados");
}

void EventMapper::loadMappingsFromConfig() {
    // Implementar carregamento de configurações do arquivo
    // Exemplo simples - na implementação final, use o ConfigManager
    if (m_configManager->hasConfig("mouse_sensitivity")) {
        m_mouseSensitivity = m_configManager->getFloatValue("mouse_sensitivity", 1.0f);
        Logger::info("Sensibilidade do mouse carregada: " + std::to_string(m_mouseSensitivity));
    }
    
    if (m_configManager->hasConfig("analog_deadzone")) {
        m_deadZone = m_configManager->getIntValue("analog_deadzone", 3200);
        Logger::info("Dead zone carregada: " + std::to_string(m_deadZone));
    }
    
    // Na implementação final, carregue também os mapeamentos de teclas, eixos, etc.
}

void EventMapper::saveMappingsToConfig() {
    // Salvar configurações atuais no ConfigManager
    m_configManager->setFloatValue("mouse_sensitivity", m_mouseSensitivity);
    m_configManager->setIntValue("analog_deadzone", m_deadZone);
    
    // Na implementação final, salve também os mapeamentos de teclas, eixos, etc.
    
    m_configManager->saveConfig();
    Logger::info("Configurações de mapeamento salvas");
}

ControllerAction EventMapper::mapEvent(const InputEvent& event) {
    // Mapeia diferentes tipos de eventos para ações do controle
    switch (event.type) {
        case InputEvent::TYPE_KEYBOARD:
            return mapKeyboardEvent(event.data.keyboard);
            
        case InputEvent::TYPE_MOUSE:
            return mapMouseEvent(event.data.mouse);
            
        default:
            return ControllerAction(); // Ação vazia
    }
}

ControllerAction EventMapper::mapKeyboardEvent(const InterceptionKeyStroke& keyStroke) {
    ControllerAction action;
    const WORD scanCode = keyStroke.code;
    const bool keyDown = !(keyStroke.state & INTERCEPTION_KEY_UP);
    
    // Atualizar estado da tecla
    m_keyStates[scanCode] = keyDown;
    
    // Verificar mapeamentos de botões
    for (const auto& mapping : m_keyMappings) {
        if (mapping.scanCode == scanCode) {
            action.type = ControllerAction::TYPE_BUTTON;
            action.data.buttonData.button = mapping.controllerButton;
            action.data.buttonData.pressed = keyDown;
            return action;
        }
    }
    
    // Verificar mapeamentos de gatilhos
    for (const auto& mapping : m_triggerMappings) {
        if (mapping.scanCode == scanCode) {
            action.type = ControllerAction::TYPE_TRIGGER;
            action.data.triggerData.trigger = mapping.trigger;
            action.data.triggerData.value = keyDown ? mapping.maxValue : 0;
            return action;
        }
    }
    
    // Para teclas de eixo, precisamos verificar o estado das duas teclas (positiva e negativa)
    for (const auto& mapping : m_axisMappings) {
        if (mapping.negativeKey == scanCode || mapping.positiveKey == scanCode) {
            return updateAxisFromKeys(mapping);
        }
    }
    
    return action; // Ação vazia se não houver mapeamento
}

ControllerAction EventMapper::updateAxisFromKeys(const AxisMapping& mapping) {
    ControllerAction action;
    action.type = ControllerAction::TYPE_AXIS;
    action.data.axisData.axis = mapping.axis;
    
    // Verificar estado das teclas negativa e positiva
    bool negKeyDown = m_keyStates[mapping.negativeKey];
    bool posKeyDown = m_keyStates[mapping.positiveKey];
    
    // Calcular valor do eixo
    if (negKeyDown && !posKeyDown) {
        // Apenas tecla negativa pressionada
        action.data.axisData.value = -mapping.maxValue;
    } else if (!negKeyDown && posKeyDown) {
        // Apenas tecla positiva pressionada
        action.data.axisData.value = mapping.maxValue;
    } else {
        // Ambas as teclas ou nenhuma tecla pressionada
        action.data.axisData.value = 0;
    }
    
    return action;
}

ControllerAction EventMapper::mapMouseEvent(const InterceptionMouseStroke& mouseStroke) {
    ControllerAction action;
    
    // Processamento de botões do mouse
    if (mouseStroke.state & INTERCEPTION_MOUSE_LEFT_BUTTON_DOWN) {
        action.type = ControllerAction::TYPE_BUTTON;
        action.data.buttonData.button = XUSB_GAMEPAD_RIGHT_THUMB; // Usar botão analógico direito
        action.data.buttonData.pressed = true;
        return action;
    }
    
    if (mouseStroke.state & INTERCEPTION_MOUSE_LEFT_BUTTON_UP) {
        action.type = ControllerAction::TYPE_BUTTON;
        action.data.buttonData.button = XUSB_GAMEPAD_RIGHT_THUMB;
        action.data.buttonData.pressed = false;
        return action;
    }
    
    if (mouseStroke.state & INTERCEPTION_MOUSE_RIGHT_BUTTON_DOWN) {
        action.type = ControllerAction::TYPE_BUTTON;
        action.data.buttonData.button = XUSB_GAMEPAD_LEFT_THUMB; // Usar botão analógico esquerdo
        action.data.buttonData.pressed = true;
        return action;
    }
    
    if (mouseStroke.state & INTERCEPTION_MOUSE_RIGHT_BUTTON_UP) {
        action.type = ControllerAction::TYPE_BUTTON;
        action.data.buttonData.button = XUSB_GAMEPAD_LEFT_THUMB;
        action.data.buttonData.pressed = false;
        return action;
    }
    
    // Processamento de movimento do mouse para eixos analógicos
    if (mouseStroke.state & INTERCEPTION_MOUSE_MOVE_RELATIVE) {
        // Calcular delta de tempo desde o último movimento
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - m_lastMouseMoveTime).count();
        m_lastMouseMoveTime = currentTime;
        
        // Evitar divisão por zero
        if (deltaTime < 0.001f) deltaTime = 0.001f;
        
        // Calcular velocidade (pixels por segundo)
        float xSpeed = static_cast<float>(mouseStroke.x) / deltaTime;
        float ySpeed = static_cast<float>(mouseStroke.y) / deltaTime;
        
        // Aplicar sensibilidade e limitar valor
        for (const auto& mapping : m_mouseMappings) {
            action.type = ControllerAction::TYPE_AXIS;
            action.data.axisData.axis = mapping.controllerAxis;
            
            if (mapping.mouseAxis == 0) { // Eixo X
                float value = xSpeed * mapping.sensitivity * m_mouseSensitivity;
                value = std::max(-32767.0f, std::min(32767.0f, value));
                action.data.axisData.value = mapping.invert ? -static_cast<short>(value) : static_cast<short>(value);
                
                // Aplicar dead zone
                if (std::abs(action.data.axisData.value) < m_deadZone) {
                    action.data.axisData.value = 0;
                }
                
                return action;
            } else if (mapping.mouseAxis == 1) { // Eixo Y
                float value = ySpeed * mapping.sensitivity * m_mouseSensitivity;
                value = std::max(-32767.0f, std::min(32767.0f, value));
                action.data.axisData.value = mapping.invert ? -static_cast<short>(value) : static_cast<short>(value);
                
                // Aplicar dead zone
                if (std::abs(action.data.axisData.value) < m_deadZone) {
                    action.data.axisData.value = 0;
                }
                
                return action;
            }
        }
    }
    
    // Processamento de rolagem do mouse
    if (mouseStroke.state & (INTERCEPTION_MOUSE_WHEEL | INTERCEPTION_MOUSE_HWHEEL)) {
        short rolling = mouseStroke.rolling;
        
        // Rolagem vertical (para gatilhos)
        if (mouseStroke.state & INTERCEPTION_MOUSE_WHEEL) {
            action.type = ControllerAction::TYPE_TRIGGER;
            
            if (rolling > 0) {
                // Rolagem para cima - Gatilho direito
                action.data.triggerData.trigger = 1;
                action.data.triggerData.value = 255; // Valor máximo
            } else {
                // Rolagem para baixo - Gatilho esquerdo
                action.data.triggerData.trigger = 0;
                action.data.triggerData.value = 255; // Valor máximo
            }
            
            return action;
        }
    }
    
    return action; // Ação vazia se não houver mapeamento
}

bool EventMapper::shouldPassThrough(const InputEvent& event) {
    // Por padrão, não passa eventos mapeados para o sistema
    // Implementação mais complexa pode permitir configurar quais eventos são passados
    
    // Se for a tecla F8 (toggle), sempre passa
    if (event.type == InputEvent::TYPE_KEYBOARD && 
        event.data.keyboard.code == DIK_F8) {
        return true;
    }
    
    // Verificar se o evento está mapeado
    ControllerAction action = mapEvent(event);
    
    // Se o evento gerar uma ação, não passa para o sistema
    if (action.type != ControllerAction::TYPE_NONE) {
        return false;
    }
    
    // Para outros eventos, passa para o sistema
    return true;
}
