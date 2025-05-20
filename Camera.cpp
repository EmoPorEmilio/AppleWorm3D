#include "Camera.h"
#include <cmath>

Camera::Camera() 
    : position(0.0f, 0.0f, 5.0f),
      target(0.0f, 0.0f, 0.0f),
      up(0.0f, 1.0f, 0.0f),
      yaw(-90.0f),
      pitch(0.0f),
      currentMode(CameraMode::FREECAM) {
    
    freeStrategy = std::make_unique<FreeCameraStrategy>();
    followStrategy = std::make_unique<FollowCameraStrategy>();
    sideStrategy = std::make_unique<SideCameraStrategy>();
    
    currentStrategy = static_cast<ICameraStrategy*>(freeStrategy.get());
}

void Camera::Initialize(float startX, float startY, float startZ) {
    position = Vector3(startX, startY, startZ);
    
    yaw = -90.0f;
    pitch = 0.0f;
    
    float yawRad = yaw * (3.14159f / 180.0f);
    float pitchRad = pitch * (3.14159f / 180.0f);
    
    Vector3 direction(
        cos(yawRad) * cos(pitchRad),
        sin(pitchRad),
        sin(yawRad) * cos(pitchRad)
    );
    
    target = position + direction;
}

void Camera::SetCharacterReference(Vector3* characterPosition) {
    followStrategy->SetCharacterReference(characterPosition);
    sideStrategy->SetCharacterReference(characterPosition);
}

void Camera::Apply() {
    gluLookAt(
        position.x, position.y, position.z,
        target.x, target.y, target.z,
        up.x, up.y, up.z
    );
}

void Camera::ToggleCameraMode() {
    Vector3 direction;
    
    switch (currentMode) {
        case CameraMode::FOLLOW_CHARACTER:
            currentMode = CameraMode::FREECAM;
            currentStrategy = static_cast<ICameraStrategy*>(freeStrategy.get());
            
            direction = target - position;
            direction.Normalize();
            
            yaw = atan2(direction.z, direction.x) * (180.0f / 3.14159f);
            
            pitch = asin(direction.y) * (180.0f / 3.14159f);
            
            break;
            
        case CameraMode::FREECAM:
            currentMode = CameraMode::SIDE_VIEW;
            currentStrategy = static_cast<ICameraStrategy*>(sideStrategy.get());
            break;
            
        case CameraMode::SIDE_VIEW:
            currentMode = CameraMode::FOLLOW_CHARACTER;
            currentStrategy = static_cast<ICameraStrategy*>(followStrategy.get());
            break;
    }
    
    Update();
}

CameraMode Camera::GetMode() const {
    return currentMode;
}

void Camera::Update() {
    if (currentStrategy) {
        currentStrategy->Update(position, target, up);
    }
}

void Camera::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_v) {
        ToggleCameraMode();
        return;
    }
    
    if (currentStrategy) {
        if (event.type == SDL_MOUSEMOTION || 
            event.type == SDL_MOUSEWHEEL || 
            event.type == SDL_KEYDOWN || 
            event.type == SDL_KEYUP) {
            currentStrategy->HandleInput(event, position, target, up, yaw, pitch);
        }
    }
}

void Camera::SetFollowDistance(float distance) {
    followStrategy->SetFollowDistance(distance);
}

void Camera::SetFollowHeight(float height) {
    followStrategy->SetHeightOffset(height);
}

void Camera::SetSideDistance(float distance) {
    sideStrategy->SetSideDistance(distance);
} 