#pragma once
#include "Vector3.h"
#include "SDL.h"

// Constantes generales para estrategias de cámara
constexpr int MIN_MOUSE_MOVEMENT = 1; // Movimiento mínimo para considerar una acción válida
constexpr Uint32 MOUSE_BUTTON_MASK = SDL_BUTTON(SDL_BUTTON_LEFT) | SDL_BUTTON(SDL_BUTTON_RIGHT);

class ICameraStrategy {
public:
    virtual ~ICameraStrategy() = default;
    
    virtual void Update(Vector3& position, Vector3& target, Vector3& up) = 0;
    
    virtual void HandleInput(const SDL_Event& event, 
                             Vector3& position, 
                             Vector3& target, 
                             Vector3& up, 
                             float& yaw, 
                             float& pitch) = 0;
                             
protected:
    // Método de utilidad para comprobar si un movimiento de ratón es válido
    bool IsValidMouseMovement(int xrel, int yrel) {
        return abs(xrel) >= MIN_MOUSE_MOVEMENT || abs(yrel) >= MIN_MOUSE_MOVEMENT;
    }
    
    // Método para comprobar si algún botón del ratón está presionado
    bool IsMouseButtonPressed(Uint32 mouseState) {
        return (mouseState & MOUSE_BUTTON_MASK) != 0;
    }
}; 