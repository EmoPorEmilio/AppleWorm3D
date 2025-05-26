#pragma once
#include "Colors.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#include "Utils.h"
#include <FreeImage.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

void drawCube(float x, float y, float z, float size, Colors color);
struct ObjVertex
{
    float x, y, z;
};

struct ObjTexCoord
{
    float u, v;
};

struct ObjNormal
{
    float nx, ny, nz;
};

struct ObjFace
{
    int v[3], t[3], n[3];
};

struct ObjModel
{
    std::vector<ObjVertex> vertices;
    std::vector<ObjTexCoord> texcoords;
    std::vector<ObjNormal> normals;
    std::vector<ObjFace> faces;
};

void drawCube(int x, int y, int z, float size, Colors color);
void DrawOBJ(const ObjModel &model, GLuint textureID);
GLuint LoadTexture(const char *filename);
bool LoadOBJ(const char *filename, ObjModel &model);
