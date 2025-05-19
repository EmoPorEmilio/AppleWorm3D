#pragma once
#include "GameObject.h"

class Terrain : public GameObject {
public:
    Terrain(Vector3 coordinates);

    void draw() override;
    void update(float deltaTime) override;
};
