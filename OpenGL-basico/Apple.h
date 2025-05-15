#pragma once
#include "GameObject.h"

class Apple : public GameObject {
public:
    Apple(Vector3 coordinates);

    void draw() override;
};
