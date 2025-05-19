#pragma once
#include "GameObject.h"

class Portal : public GameObject {
public:
    Portal(Vector3 coordinates);

    void draw() override;
    void update(float deltaTime) override;
};
