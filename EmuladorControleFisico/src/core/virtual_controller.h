
/**
 * @file virtual_controller.h
 * @brief Interface para emulação de controle virtual usando ViGEm
 */

#pragma once

#include <Windows.h>
#include "../lib/ViGEm/Client.h"
#include <string>

/**
 * @struct ControllerAction
 * @brief Representa uma ação a ser aplicada ao controle virtual
 */
struct ControllerAction {
    enum ActionType {
        TYPE_NONE,
        TYPE_BUTTON,
        TYPE_AXIS,
        TYPE_TRIGGER
    };
    
    ActionType type;
    
    union {
        struct {
            XUSB_BUTTON button;  // Botão Xbox (definido em ViGEm)
            bool pressed;        // true = pressionado, false = solto
        } buttonData;
        
        struct {
            int axis;            // 0 = left X, 1 = left Y, 2 = right X, 3 = right Y
            short value;         // -32768 a 32767
        } axisData;
        
        struct {
            int trigger;         // 0 = left, 1 = right
            BYTE value;          // 0 a 255
        } triggerData;
    } data;
    
    ControllerAction() : type(TYPE_NONE) {
        memset(&data, 0, sizeof(data));
    }
};

/**
 * @class VirtualController
 * @brief Gerencia a emulação de um controle Xbox 360 virtual usando ViGEm
 */
class VirtualController {
public:
    /**
     * @brief Construtor
     */
    VirtualController();
    
    /**
     * @brief Destrutor
     */
    ~VirtualController();
    
    /**
     * @brief Inicializa o controle virtual
     * @return true se inicializado com sucesso, false caso contrário
     */
    bool initialize();
    
    /**
     * @brief Aplica uma ação ao controle virtual
     * @param action Ação a ser aplicada
     * @return true se aplicada com sucesso, false caso contrário
     */
    bool applyAction(const ControllerAction& action);
    
    /**
     * @brief Define o estado de um botão do controle
     * @param button Código do botão (definido pela ViGEm)
     * @param pressed Estado do botão (true = pressionado, false = solto)
     * @return true se definido com sucesso, false caso contrário
     */
    bool setButton(XUSB_BUTTON button, bool pressed);
    
    /**
     * @brief Define o valor de um eixo analógico
     * @param axis Índice do eixo (0=left X, 1=left Y, 2=right X, 3=right Y)
     * @param value Valor do eixo (-32768 a 32767)
     * @return true se definido com sucesso, false caso contrário
     */
    bool setAxis(int axis, SHORT value);
    
    /**
     * @brief Define o valor de um gatilho (trigger)
     * @param trigger Índice do gatilho (0=left, 1=right)
     * @param value Valor do gatilho (0 a 255)
     * @return true se definido com sucesso, false caso contrário
     */
    bool setTrigger(int trigger, BYTE value);

private:
    PVIGEM_CLIENT m_client;
    PVIGEM_TARGET m_target;
    XUSB_REPORT m_report;
    bool m_initialized;
    bool m_connected;
    
    /**
     * @brief Submete o relatório atual para o controle virtual
     * @return true se enviado com sucesso, false caso contrário
     */
    bool submitReport();
};
