#define _CRT_SECURE_NO_WARNINGS
#include "Utils.h"
#include "Colors.h" 
#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#include <FreeImage.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>

GLuint LoadTexture(const char* filename) {
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
    FIBITMAP* dib = FreeImage_Load(fif, filename, 0);
    FIBITMAP* temp = dib;
    dib = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(temp);

    int width = FreeImage_GetWidth(dib);
    int height = FreeImage_GetHeight(dib);
    GLubyte* textureData = FreeImage_GetBits(dib);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_BGRA, GL_UNSIGNED_BYTE, textureData);

    FreeImage_Unload(dib);

    return texID;
}

bool LoadOBJ(const char* filename, ObjModel& model) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            ObjVertex v;
            iss >> v.x >> v.y >> v.z;
            model.vertices.push_back(v);
        }
        else if (type == "vt") {
            ObjTexCoord t;
            iss >> t.u >> t.v;
            model.texcoords.push_back(t);
        }
        else if (type == "vn") {
            ObjNormal n;
            iss >> n.nx >> n.ny >> n.nz;
            model.normals.push_back(n);
        }
        else if (type == "f") {
            std::vector<std::string> tokens;
            std::string token;
            while (iss >> token) tokens.push_back(token);
            if (tokens.size() < 3) continue; // skip non-triangle faces

            ObjFace f;

            // Helper lambda for negative indices (OBJ supports them)
            auto fix_index = [](int idx, int size) {
                return idx > 0 ? idx - 1 : (idx < 0 ? size + idx : -1);
                };

            for (int i = 0; i < 3; ++i) {
                int v = 0, t = 0, n = 0;
                // Try to parse v/t/n, v//n, v/t, or v
                if (sscanf(tokens[i].c_str(), "%d/%d/%d", &v, &t, &n) == 3) {
                    f.v[i] = fix_index(v, model.vertices.size());
                    f.t[i] = fix_index(t, model.texcoords.size());
                    f.n[i] = fix_index(n, model.normals.size());
                }
                else if (sscanf(tokens[i].c_str(), "%d//%d", &v, &n) == 2) {
                    f.v[i] = fix_index(v, model.vertices.size());
                    f.t[i] = -1;
                    f.n[i] = fix_index(n, model.normals.size());
                }
                else if (sscanf(tokens[i].c_str(), "%d/%d", &v, &t) == 2) {
                    f.v[i] = fix_index(v, model.vertices.size());
                    f.t[i] = fix_index(t, model.texcoords.size());
                    f.n[i] = -1;
                }
                else if (sscanf(tokens[i].c_str(), "%d", &v) == 1) {
                    f.v[i] = fix_index(v, model.vertices.size());
                    f.t[i] = -1;
                    f.n[i] = -1;
                }
                else {
                    std::cerr << "Unrecognized face format: " << tokens[i] << std::endl;
                    f.v[i] = f.t[i] = f.n[i] = -1;
                }
            }

            // Debug: print if any indices are out of range
            for (int i = 0; i < 3; ++i) {
                if (f.v[i] < 0 || f.v[i] >= (int)model.vertices.size()) {
                    printf("Invalid vertex index: %d (face token: %s)\n", f.v[i], tokens[i].c_str());
                }
                if (f.t[i] < -1 || f.t[i] >= (int)model.texcoords.size()) {
                    printf("Invalid texcoord index: %d (face token: %s)\n", f.t[i], tokens[i].c_str());
                }
                if (f.n[i] < -1 || f.n[i] >= (int)model.normals.size()) {
                    printf("Invalid normal index: %d (face token: %s)\n", f.n[i], tokens[i].c_str());
                }
            }

            model.faces.push_back(f);
        }
    }
    return true;
}

//void drawCube(int x, int y, int z, float size, Colors color) {
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, colors_table[color]);
//
//    glPushMatrix();
//    glTranslatef(0.5f, 0.5f, 0.5f);
//    glTranslatef(float(x), float(y), float(z));
//    glScalef(size, size, size);
//
//    glBegin(GL_QUADS);
//    glNormal3f(0, 0, 1);
//    glVertex3f(-0.5f, -0.5f, 0.5f);
//    glVertex3f(0.5f, -0.5f, 0.5f);
//    glVertex3f(0.5f, 0.5f, 0.5f);
//    glVertex3f(-0.5f, 0.5f, 0.5f);
//    glNormal3f(0, 0, -1);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, 0.5f, -0.5f);
//    glVertex3f(0.5f, 0.5f, -0.5f);
//    glVertex3f(0.5f, -0.5f, -0.5f);
//
//    glNormal3f(-1, 0, 0);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, -0.5f, 0.5f);
//    glVertex3f(-0.5f, 0.5f, 0.5f);
//    glVertex3f(-0.5f, 0.5f, -0.5f);
//
//    glNormal3f(1, 0, 0);
//    glVertex3f(0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, 0.5f, -0.5f);
//    glVertex3f(0.5f, 0.5f, 0.5f);
//    glVertex3f(0.5f, -0.5f, 0.5f);
//
//    glNormal3f(0, 1, 0);
//    glVertex3f(-0.5f, 0.5f, -0.5f);
//    glVertex3f(-0.5f, 0.5f, 0.5f);
//    glVertex3f(0.5f, 0.5f, 0.5f);
//    glVertex3f(0.5f, 0.5f, -0.5f);
//
//    glNormal3f(0, -1, 0);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, -0.5f, -0.5f);
//    glVertex3f(0.5f, -0.5f, 0.5f);
//    glVertex3f(-0.5f, -0.5f, 0.5f);
//    glEnd();
//
//    glPopMatrix();
//}

void drawCube(float x, float y, float z, float size, Colors color) {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colors_table[color]);

    glPushMatrix();
    glTranslatef(0.5f, 0.5f, 0.5f);
    glTranslatef(x, y, z);
    glScalef(size, size, size);

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glNormal3f(0, 0, -1);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glNormal3f(-1, 0, 0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glNormal3f(1, 0, 0);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    glNormal3f(0, 1, 0);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glNormal3f(0, -1, 0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    glPopMatrix();
}

void DrawOBJ(const ObjModel& model, GLuint textureID) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLES);
    for (const auto& face : model.faces) {
        for (int i = 0; i < 3; ++i) {
            // Only use normal if valid
            if (face.n[i] >= 0 && face.n[i] < (int)model.normals.size()) {
                const ObjNormal& n = model.normals[face.n[i]];
                glNormal3f(n.nx, n.ny, n.nz);
            }
            // Only use texcoord if valid
            if (face.t[i] >= 0 && face.t[i] < (int)model.texcoords.size()) {
                const ObjTexCoord& t = model.texcoords[face.t[i]];
                glTexCoord2f(t.u, t.v); // Flip V for OpenGL
            }
            else {
                // If missing, use a default texcoord
                glTexCoord2f(0.0f, 0.0f);
            }
            // Always use vertex
            if (face.v[i] >= 0 && face.v[i] < (int)model.vertices.size()) {
                const ObjVertex& v = model.vertices[face.v[i]];
                glVertex3f(v.x, v.y, v.z);
            }
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}