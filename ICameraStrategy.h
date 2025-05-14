#pragma once
#include "Vector3.h"
#include "SDL.h"

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
}; 