#pragma once
#include "ICameraStrategy.h"
#include <cmath>

constexpr float PI_FOLLOW = 3.14159265359f;
constexpr float TWOPI_FOLLOW = 2.0f * PI_FOLLOW;
constexpr float DEG2RAD_FOLLOW = PI_FOLLOW / 180.0f;

class FollowCameraStrategy : public ICameraStrategy {
private:
    Vector3* characterPosition;
    
    float followDistance;
    float heightOffset;
    const float mouseSensitivity = 0.2f;
    const float zoomSpeed = 0.5f;
    const float minDistance = 2.0f;
    const float maxDistance = 15.0f;
    

    float localYaw = -90.0f;  
    float localPitch = 20.0f; 
public:
    FollowCameraStrategy(float followDistance = 5.0f, float heightOffset = 2.0f)
        : characterPosition(nullptr), followDistance(followDistance), heightOffset(heightOffset) {}
    
    void SetCharacterReference(Vector3* position) {
        characterPosition = position;
    }
    
    virtual void Update(Vector3& position, Vector3& target, Vector3& up) override {
        if (!characterPosition) return;
        
        float yawRad = localYaw * DEG2RAD_FOLLOW;
        float pitchRad = localPitch * DEG2RAD_FOLLOW;
        
        Vector3 offset(
            -sin(yawRad) * cos(pitchRad) * followDistance,
            sin(pitchRad) * followDistance + heightOffset,
            -cos(yawRad) * cos(pitchRad) * followDistance
        );
        
        position = *characterPosition + offset;
        target = *characterPosition + Vector3(0.0f, 1.0f, 0.0f); 
    }
    
    virtual void HandleInput(const SDL_Event& event, 
                           Vector3& position, 
                           Vector3& target, 
                           Vector3& up, 
                           float& cameraYaw, 
                           float& cameraPitch) override {
        if (event.type == SDL_MOUSEWHEEL) {
            followDistance -= event.wheel.y * zoomSpeed;
            
            if (followDistance < minDistance) followDistance = minDistance;
            if (followDistance > maxDistance) followDistance = maxDistance;
            
            Update(position, target, up);
        }
        
        if (event.type == SDL_MOUSEMOTION) {
            Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
            if ((mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) || 
                (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
                localYaw -= event.motion.xrel * mouseSensitivity;
                localPitch += event.motion.yrel * mouseSensitivity;
                
                while (localYaw >= 360.0f) localYaw -= 360.0f;
                while (localYaw < 0.0f) localYaw += 360.0f;
                
                if (localPitch > 85.0f) localPitch = 85.0f;
                if (localPitch < -45.0f) localPitch = -45.0f;
                
                cameraYaw = localYaw;
                cameraPitch = localPitch;
                
                Update(position, target, up);
            }
        }
    }
    
    void SetFollowDistance(float distance) {
        followDistance = distance;
        
        if (followDistance < minDistance) followDistance = minDistance;
        if (followDistance > maxDistance) followDistance = maxDistance;
    }
    
    void SetHeightOffset(float height) {
        heightOffset = height;
    }
}; 