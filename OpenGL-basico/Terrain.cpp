#pragma once
#include "Terrain.h"
#include "Vector3.h"
#include "Colors.h"
#include "Utils.h"

void Terrain::draw() {
    GLfloat default_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat default_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat default_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat default_shi[] = { 0.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, default_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, default_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, default_spe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, default_shi);
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    Vector3 position = this->GetPosition();
    glTranslatef(position.x + 0.5, position.y + 0.7, position.z + 0.5);
    glScalef(0.5f, 0.5f, 0.5f);
    DrawOBJ(terrainModel, terrainTexture);
    glPopMatrix();
}

void Terrain::update(float deltaTime) {
}


Terrain::Terrain(Vector3 coordinates, const char* objPath, const char* texPath) : GameObject(coordinates) {
    glShadeModel(GL_SMOOTH);
    LoadOBJ(objPath, terrainModel);
    terrainTexture = LoadTexture(texPath);