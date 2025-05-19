#pragma once
#include "GameObject.h"
#include "Animator.h"

class Apple : public GameObject {
private:
    Animator* anim;
    bool up;
    Vector3 logicPos;
public:
    Apple(Vector3 coordinates);

    void draw() override;
    void update(float deltaTime) override;
};
