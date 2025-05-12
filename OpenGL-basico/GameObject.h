#pragma once
#include "Coordinates3D.h"

class GameObject {
public:
    Coordinates3D position;

    GameObject(const Coordinates3D& pos) : position(pos) {}
    virtual ~GameObject() = default;

    virtual void draw() = 0;
};