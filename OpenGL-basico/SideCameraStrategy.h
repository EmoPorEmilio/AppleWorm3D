#pragma once
#include "ICameraStrategy.h"
#include <cmath>

class SideCameraStrategy : public ICameraStrategy {
private:
    const float moveSpeed = 0.1f;
    const float mouseSensitivity = 0.2f;
    const float zoomSpeed = 0.5f;
    const float minDistance = 3.0f;
    const float maxDistance = 15.0f;

    float sideDistance = 8.0f;
    float heightOffset = 1.0f;

    Vector3* characterRef = nullptr;

public:
    SideCameraStrategy() = default;

    void SetCharacterReference(Vector3* characterPosition) {
        characterRef = characterPosition;
    }

    virtual void Update(Vector3& position, Vector3& target, Vector3& up) override {
        if (!characterRef) return;

        position.x = characterRef->x + sideDistance;
        position.y = characterRef->y + heightOffset;
        position.z = characterRef->z;

        target.x = characterRef->x;
        target.y = characterRef->y + heightOffset;
        target.z = characterRef->z;

        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
    }

    virtual void HandleInput(const SDL_Event& event,
        Vector3& position,
        Vector3& target,
        Vector3& up,
        float& cameraYaw,
        float& cameraPitch) override {
        if (event.type == SDL_MOUSEWHEEL) {
            sideDistance -= event.wheel.y * zoomSpeed;

            if (sideDistance < minDistance) sideDistance = minDistance;
            if (sideDistance > maxDistance) sideDistance = maxDistance;

            Update(position, target, up);
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_q: heightOffset += 0.1f; Update(position, target, up); break;
            case SDLK_e: heightOffset -= 0.1f; Update(position, target, up); break;

            case SDLK_r:
                sideDistance = -sideDistance;
                Update(position, target, up);
                break;
            }
        }
    }

    void SetSideDistance(float distance) {
        sideDistance = distance;

        if (sideDistance < minDistance) sideDistance = minDistance;
        if (sideDistance > maxDistance) sideDistance = maxDistance;
    }

    void SetHeightOffset(float height) {
        heightOffset = height;
    }
};
