#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "Utils.h"

class Apple : public GameObject
{
private:
    Animator *anim;
    bool up;
    Vector3 logicPos;

public:
    ObjModel appleModel;
    GLuint appleTexture;
    Apple(Vector3 coordinates, const char *objPath, const char *texPath);

    void draw() override;
    void update(float deltaTime) override;
};
