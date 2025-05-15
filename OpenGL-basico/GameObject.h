#pragma once
#include "Vector3.h"

class GameObject {
private:
    Vector3 position;
    bool animating;
public:
    GameObject(Vector3 position);
    virtual ~GameObject() = default;
	void SetPosition(Vector3 position);
	Vector3 GetPosition();
    virtual void draw() = 0;
};