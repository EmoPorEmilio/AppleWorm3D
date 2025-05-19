#pragma once
#include "GameObject.h"
#include "Utils.h"

class Portal : public GameObject {
public:
    ObjModel portalModel;
    GLuint portalTexture;
    Portal(Vector3 coordinates, const char* objPath, const char* texPath);

    void draw() override;
};
