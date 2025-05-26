#pragma once
#include "GameObject.h"
#include "Utils.h"

class Terrain : public GameObject {
public:
    ObjModel terrainModel;
    GLuint terrainTexture;
    Terrain(Vector3 coordinates, const char* objPath, const char* texPath);

    void draw() override;
    void update(float deltaTime) override;
};
