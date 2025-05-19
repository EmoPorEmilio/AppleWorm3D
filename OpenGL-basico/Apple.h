#pragma once
#include "GameObject.h"
#include "Utils.h"

class Apple : public GameObject {
public:
    ObjModel appleModel;
    GLuint appleTexture;
    Apple(Vector3 coordinates, const char* objPath, const char* texPath);

    void draw() override;
};
