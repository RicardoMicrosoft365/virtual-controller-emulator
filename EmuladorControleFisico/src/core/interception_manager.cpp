
/**
 * @file interception_manager.cpp
 * @brief Implementação do gerenciador de interceptação de eventos
 */

#include "interception_manager.h"
#include "../utils/logger.h"

InterceptionManager::InterceptionManager() 
    : m_context(nullptr), m_initialized(false) {
}

InterceptionManager::~InterceptionManager() {
    if (m_initialized && m_context) {
        interception_destroy_context(m_context);
        Logger::info("Contexto Interception destruído");
    }
}

bool InterceptionManager::initialize() {
    // Criar contexto de interceptação
    m_context = interception_create_context();
    
    if (!m_context) {
        Logger::error("Falha ao criar contexto Interception");
        return false;
    }
    
    m_initialized = true;
    Logger::info("Contexto Interception criado com sucesso");
    return true;
}

void InterceptionManager::setKeyboardFilter(InterceptionFilter filter) {
    if (!m_initialized) {
        Logger::warning("Tentativa de definir filtro de teclado sem inicialização");
        return;
    }
    
    interception_set_filter(m_context, interception_is_keyboard, filter);
    Logger::info("Filtro de teclado definido");
}

void InterceptionManager::setMouseFilter(InterceptionFilter filter) {
    if (!m_initialized) {
        Logger::warning("Tentativa de definir filtro de mouse sem inicialização");
        return;
    }
    
    interception_set_filter(m_context, interception_is_mouse, filter);
    Logger::info("Filtro de mouse definido");
}

InputEvent InterceptionManager::waitForEvent(int timeout) {
    InputEvent event;
    
    if (!m_initialized) {
        Logger::warning("Tentativa de esperar por evento sem inicialização");
        return event;
    }
    
    InterceptionDevice device;
    
    if (timeout < 0) {
        // Espera infinita
        device = interception_wait(m_context);
    } else {
        // Espera com timeout
        device = interception_wait_with_timeout(m_context, timeout);
    }
    
    if (interception_is_invalid(device)) {
        // Nenhum evento disponível (timeout)
        return event;
    }
    
    event.deviceId = device;
    
    // Determinar o tipo de dispositivo e ler o evento
    if (interception_is_keyboard(device)) {
        event.type = InputEvent::TYPE_KEYBOARD;
        interception_receive(m_context, device, (InterceptionStroke*)&event.data.keyboard, 1);
    } else if (interception_is_mouse(device)) {
        event.type = InputEvent::TYPE_MOUSE;
        interception_receive(m_context, device, (InterceptionStroke*)&event.data.mouse, 1);
    }
    
    return event;
}

void InterceptionManager::passEventThrough(const InputEvent& event) {
    if (!m_initialized) {
        Logger::warning("Tentativa de passar evento sem inicialização");
        return;
    }
    
    if (event.type == InputEvent::TYPE_KEYBOARD) {
        interception_send(m_context, event.deviceId, (const InterceptionStroke*)&event.data.keyboard, 1);
    } else if (event.type == InputEvent::TYPE_MOUSE) {
        interception_send(m_context, event.deviceId, (const InterceptionStroke*)&event.data.mouse, 1);
    }
}

void InterceptionManager::sendKeyboardEvent(const InterceptionKeyStroke& keyStroke) {
    if (!m_initialized) {
        Logger::warning("Tentativa de enviar evento de teclado sem inicialização");
        return;
    }
    
    // Encontrar o primeiro dispositivo de teclado para enviar o evento
    for (InterceptionDevice device = 1; device <= INTERCEPTION_MAX_KEYBOARD; device++) {
        if (interception_is_keyboard(device)) {
            interception_send(m_context, device, (const InterceptionStroke*)&keyStroke, 1);
            break;
        }
    }
}

void InterceptionManager::sendMouseEvent(const InterceptionMouseStroke& mouseStroke) {
    if (!m_initialized) {
        Logger::warning("Tentativa de enviar evento de mouse sem inicialização");
        return;
    }
    
    // Encontrar o primeiro dispositivo de mouse para enviar o evento
    for (InterceptionDevice device = INTERCEPTION_MAX_KEYBOARD + 1; 
         device <= INTERCEPTION_MAX_KEYBOARD + INTERCEPTION_MAX_MOUSE; device++) {
        if (interception_is_mouse(device)) {
            interception_send(m_context, device, (const InterceptionStroke*)&mouseStroke, 1);
            break;
        }
    }
}
