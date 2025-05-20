#pragma once
#include "GameObject.h"
#include "Utils.h"

class Lava : public GameObject {
public:
    ObjModel lavaModel;
    GLuint lavaTexture;
    Lava(Vector3 coordinates, const char* objPath, const char* texPath);

    void draw() override;
    void update(float deltaTime) override;
};
