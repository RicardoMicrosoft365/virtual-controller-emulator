
/**
 * @file event_mapper.h
 * @brief Mapeia eventos de entrada para ações do controle virtual
 */

#pragma once

#include "interception_manager.h"
#include "virtual_controller.h"
#include "../utils/config_manager.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>

/**
 * @struct KeyMapping
 * @brief Define o mapeamento de uma tecla para uma ação do controle
 */
struct KeyMapping {
    WORD scanCode;                 // Código da tecla
    XUSB_BUTTON controllerButton;  // Botão do controle
    
    KeyMapping(WORD sc, XUSB_BUTTON cb) 
        : scanCode(sc), controllerButton(cb) {}
};

/**
 * @struct AxisMapping
 * @brief Define o mapeamento de teclas para um eixo analógico
 */
struct AxisMapping {
    WORD negativeKey;  // Tecla para valor negativo (ex: esquerda, cima)
    WORD positiveKey;  // Tecla para valor positivo (ex: direita, baixo)
    int axis;          // Índice do eixo (0=LX, 1=LY, 2=RX, 3=RY)
    int maxValue;      // Valor máximo absoluto (0 a 32767)
    
    AxisMapping(WORD nk, WORD pk, int a, int mv = 32767) 
        : negativeKey(nk), positiveKey(pk), axis(a), maxValue(mv) {}
};

/**
 * @struct TriggerMapping
 * @brief Define o mapeamento de uma tecla para um gatilho
 */
struct TriggerMapping {
    WORD scanCode;     // Código da tecla
    int trigger;       // Índice do gatilho (0=L, 1=R)
    BYTE maxValue;     // Valor máximo (0 a 255)
    
    TriggerMapping(WORD sc, int t, BYTE mv = 255) 
        : scanCode(sc), trigger(t), maxValue(mv) {}
};

/**
 * @struct MouseAxisMapping
 * @brief Define o mapeamento do movimento do mouse para um eixo analógico
 */
struct MouseAxisMapping {
    int mouseAxis;     // Eixo do mouse (0=X, 1=Y)
    int controllerAxis; // Eixo do controle (0=LX, 1=LY, 2=RX, 3=RY)
    float sensitivity;  // Sensibilidade
    bool invert;        // Inverter direção
    
    MouseAxisMapping(int ma, int ca, float s = 1.0f, bool i = false) 
        : mouseAxis(ma), controllerAxis(ca), sensitivity(s), invert(i) {}
};

/**
 * @class EventMapper
 * @brief Mapeia eventos de dispositivos de entrada para ações do controle virtual
 */
class EventMapper {
public:
    /**
     * @brief Construtor
     * @param configManager Referência para o gerenciador de configurações
     */
    EventMapper(ConfigManager* configManager);
    
    /**
     * @brief Define o mapeamento de teclas para botões do controle
     * @param mappings Lista de mapeamentos de teclas
     */
    void setKeyMappings(const std::vector<KeyMapping>& mappings);
    
    /**
     * @brief Define o mapeamento de teclas para eixos analógicos
     * @param mappings Lista de mapeamentos de eixos
     */
    void setAxisMappings(const std::vector<AxisMapping>& mappings);
    
    /**
     * @brief Define o mapeamento de teclas para gatilhos
     * @param mappings Lista de mapeamentos de gatilhos
     */
    void setTriggerMappings(const std::vector<TriggerMapping>& mappings);
    
    /**
     * @brief Define o mapeamento de movimentos do mouse para eixos do controle
     * @param mappings Lista de mapeamentos de mouse
     */
    void setMouseMappings(const std::vector<MouseAxisMapping>& mappings);
    
    /**
     * @brief Mapeia um evento de entrada para uma ação do controle
     * @param event Evento de entrada a ser mapeado
     * @return Ação do controle correspondente
     */
    ControllerAction mapEvent(const InputEvent& event);
    
    /**
     * @brief Determina se um evento deve ser passado para o sistema operacional
     * @param event Evento a ser verificado
     * @return true se o evento deve ser passado, false caso contrário
     */
    bool shouldPassThrough(const InputEvent& event);
    
    /**
     * @brief Carrega os mapeamentos a partir do gerenciador de configurações
     */
    void loadMappingsFromConfig();
    
    /**
     * @brief Salva os mapeamentos atuais no gerenciador de configurações
     */
    void saveMappingsToConfig();

private:
    ConfigManager* m_configManager;
    
    // Mapeamentos
    std::vector<KeyMapping> m_keyMappings;
    std::vector<AxisMapping> m_axisMappings;
    std::vector<TriggerMapping> m_triggerMappings;
    std::vector<MouseAxisMapping> m_mouseMappings;
    
    // Estado das teclas para eixos analógicos
    std::unordered_map<WORD, bool> m_keyStates;
    
    // Último tempo em que processamos eventos
    std::chrono::steady_clock::time_point m_lastMouseMoveTime;
    
    // Sensibilidade do mouse
    float m_mouseSensitivity;
    
    // Dead zone para analógicos
    int m_deadZone;
    
    /**
     * @brief Mapeia evento de teclado
     * @param keyStroke Evento de teclado
     * @return Ação do controle
     */
    ControllerAction mapKeyboardEvent(const InterceptionKeyStroke& keyStroke);
    
    /**
     * @brief Mapeia evento de mouse
     * @param mouseStroke Evento de mouse
     * @return Ação do controle
     */
    ControllerAction mapMouseEvent(const InterceptionMouseStroke& mouseStroke);
    
    /**
     * @brief Atualiza o estado de um eixo analógico com base no estado das teclas
     * @param mapping Mapeamento do eixo
     * @return Ação do controle para o eixo
     */
    ControllerAction updateAxisFromKeys(const AxisMapping& mapping);
};
