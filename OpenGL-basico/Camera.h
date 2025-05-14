#pragma once
#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#include "Vector3.h"
#include "ICameraStrategy.h"
#include "FreeCameraStrategy.h"
#include "FollowCameraStrategy.h"
#include "SideCameraStrategy.h"
#include <memory>

enum class CameraMode {
    FREECAM,
    FOLLOW_CHARACTER,
    SIDE_VIEW
};

class Camera {
private:
    Vector3 position;
    Vector3 target;
    Vector3 up;

    float yaw;
    float pitch;

    ICameraStrategy* currentStrategy;

    std::unique_ptr<FreeCameraStrategy> freeStrategy;
    std::unique_ptr<FollowCameraStrategy> followStrategy;
    std::unique_ptr<SideCameraStrategy> sideStrategy;

    CameraMode currentMode;

public:
    Camera();

    virtual ~Camera() = default;

    void Initialize(float startX, float startY, float startZ);

    void SetCharacterReference(Vector3* characterPosition);

    void Apply();

    void ToggleCameraMode();

    CameraMode GetMode() const;

    void Update();

    void HandleEvent(const SDL_Event& event);

    void SetFollowDistance(float distance);

    void SetFollowHeight(float height);

    void SetSideDistance(float distance);
};
