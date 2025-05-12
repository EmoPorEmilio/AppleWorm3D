#pragma once
#include <memory>
#include "GameObject.h"

class CubeGridElement {
public:
    int x, y, z;
    std::shared_ptr<GameObject> object;

    CubeGridElement(int x, int y, int z)
        : x(x), y(y), z(z), object(nullptr) {
    }
};