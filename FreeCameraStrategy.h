#pragma once
#include "ICameraStrategy.h"
#include <cmath>

// Constantes
constexpr float PI = 3.14159265359f;
constexpr float TWOPI = 2.0f * PI;
constexpr float DEG2RAD = PI / 180.0f;

class FreeCameraStrategy : public ICameraStrategy {
private:
    const float moveSpeed = 0.1f;
    const float mouseSensitivity = 0.2f;
    const float zoomSpeed = 0.5f;     
    const float minDistance = 1.0f;    
    const float maxDistance = 20.0f;   
    float localYaw = -90.0f;  
    float localPitch = 0.0f;
    
    float distance = 5.0f;

public:
    virtual void Update(Vector3& position, Vector3& target, Vector3& up) override {
        // Al entrar a este modo, se mantiene la posición y dirección actuales
        // La actualización de la posición/dirección se hace en HandleInput
    }
    
    virtual void HandleInput(const SDL_Event& event, 
                           Vector3& position, 
                           Vector3& target, 
                           Vector3& up, 
                           float& cameraYaw, 
                           float& cameraPitch) override {
        // Sincronizar valores locales con los de la cámara principal
        if (localYaw != cameraYaw || localPitch != cameraPitch) {
            localYaw = cameraYaw;
            localPitch = cameraPitch;
        }
        
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_w: MoveForward(position, target, moveSpeed); break;
                case SDLK_s: MoveForward(position, target, -moveSpeed); break;
                case SDLK_a: MoveRight(position, target, up, -moveSpeed); break;
                case SDLK_d: MoveRight(position, target, up, moveSpeed); break;
                case SDLK_q: MoveUp(position, target, up, moveSpeed); break;
                case SDLK_e: MoveUp(position, target, up, -moveSpeed); break;
            }
        }
        
        if (event.type == SDL_MOUSEWHEEL) {
            float zoomAmount = event.wheel.y * zoomSpeed;
            
            Vector3 direction = target - position;
            float currentDistance = direction.Length();
            direction.Normalize();
            
            currentDistance -= zoomAmount;
            if (currentDistance < minDistance) currentDistance = minDistance;
            if (currentDistance > maxDistance) currentDistance = maxDistance;
            
            position = target - direction * currentDistance;
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
            if ((mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) || 
                (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
                cameraYaw += -event.motion.xrel * mouseSensitivity;
                cameraPitch += -event.motion.yrel * mouseSensitivity;
                
                while (cameraYaw >= 360.0f) cameraYaw -= 360.0f;
                while (cameraYaw < 0.0f) cameraYaw += 360.0f;
                
                if (cameraPitch > 89.0f) cameraPitch = 89.0f;
                if (cameraPitch < -89.0f) cameraPitch = -89.0f;
                
                localYaw = cameraYaw;
                localPitch = cameraPitch;
                
                UpdateDirection(position, target, cameraYaw, cameraPitch);
            }
        }
    }
    
private:
    void MoveForward(Vector3& position, Vector3& target, float amount) {
        Vector3 direction = target - position;
        direction.Normalize();
        
        position += direction * amount;
        target += direction * amount;
    }
    
    void MoveRight(Vector3& position, Vector3& target, const Vector3& up, float amount) {
        Vector3 direction = target - position;
        Vector3 right = direction.Cross(up);
        right.Normalize();
        
        position += right * amount;
        target += right * amount;
    }
    
    void MoveUp(Vector3& position, Vector3& target, const Vector3& up, float amount) {
        Vector3 upNormalized = Vector3(up.x, up.y, up.z);
        upNormalized.Normalize();
        
        position += upNormalized * amount;
        target += upNormalized * amount;
    }
    
    void UpdateDirection(Vector3& position, Vector3& target, float yaw, float pitch) {
        float yawRad = fmod(yaw, 360.0f) * DEG2RAD;
        float pitchRad = pitch * DEG2RAD;
        
        Vector3 direction;
        direction.x = cos(yawRad) * cos(pitchRad);
        direction.y = sin(pitchRad);
        direction.z = sin(yawRad) * cos(pitchRad);
        direction.Normalize();
        
        Vector3 currentDirection = target - position;
        float currentDistance = currentDirection.Length();
        
        target = position + direction * currentDistance;
    }
}; 