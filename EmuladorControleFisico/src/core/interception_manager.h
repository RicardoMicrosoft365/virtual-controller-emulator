
/**
 * @file interception_manager.h
 * @brief Gerenciador de interceptação de eventos de teclado e mouse
 */

#pragma once

#include <Windows.h>
#include "../lib/interception/interception.h"
#include <vector>

/**
 * @struct InputEvent
 * @brief Representa um evento de entrada (teclado ou mouse)
 */
struct InputEvent {
    enum EventType {
        TYPE_NONE,
        TYPE_KEYBOARD,
        TYPE_MOUSE
    };
    
    EventType type;
    InterceptionDevice deviceId;
    union {
        InterceptionKeyStroke keyboard;
        InterceptionMouseStroke mouse;
    } data;
    
    InputEvent() : type(TYPE_NONE), deviceId(0) { 
        memset(&data, 0, sizeof(data)); 
    }
};

/**
 * @class InterceptionManager
 * @brief Gerencia a interceptação de eventos de entrada usando a biblioteca Interception
 */
class InterceptionManager {
public:
    /**
     * @brief Construtor
     */
    InterceptionManager();
    
    /**
     * @brief Destrutor
     */
    ~InterceptionManager();
    
    /**
     * @brief Inicializa o gerenciador de interceptação
     * @return true se inicializado com sucesso, false caso contrário
     */
    bool initialize();
    
    /**
     * @brief Define o filtro para eventos de teclado
     * @param filter Filtro de eventos de teclado
     */
    void setKeyboardFilter(InterceptionFilter filter);
    
    /**
     * @brief Define o filtro para eventos de mouse
     * @param filter Filtro de eventos de mouse
     */
    void setMouseFilter(InterceptionFilter filter);
    
    /**
     * @brief Aguarda por um evento de entrada
     * @param timeout Tempo máximo de espera em milissegundos (-1 para infinito)
     * @return Evento de entrada capturado
     */
    InputEvent waitForEvent(int timeout = -1);
    
    /**
     * @brief Permite que um evento passe para o sistema operacional
     * @param event Evento a ser passado
     */
    void passEventThrough(const InputEvent& event);
    
    /**
     * @brief Envia um evento de teclado para o sistema
     * @param keyStroke Estrutura de evento de teclado
     */
    void sendKeyboardEvent(const InterceptionKeyStroke& keyStroke);
    
    /**
     * @brief Envia um evento de mouse para o sistema
     * @param mouseStroke Estrutura de evento de mouse
     */
    void sendMouseEvent(const InterceptionMouseStroke& mouseStroke);

private:
    InterceptionContext m_context;
    bool m_initialized;
};
