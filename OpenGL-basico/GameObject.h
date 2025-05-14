#pragma once
#include "Vector3.h"

class GameObject {
public:
    Vector3 position;

    GameObject(Vector3 position);
    virtual ~GameObject() = default;

    virtual void draw() = 0;
};