#pragma once
#include "Apple.h"
#include "Vector3.h"
#include "Colors.h"
#include "Utils.h"

void Apple::update(float deltaTime)
{
    if (!this->anim->IsAnimating())
    {
        if (up)
        {
            anim->AnimateLinear(logicPos, logicPos + Vector3(0, 0.1, 0), 5);
        }
        else
        {
            anim->AnimateLinear(logicPos + Vector3(0, 0.1, 0), logicPos, 5);
        }
        up = !up;
    }
    anim->Update(deltaTime);
}

void Apple::draw()
{
    GLfloat mat_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_dif[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_spe[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shi[] = {64.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shi);
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    Vector3 position = this->GetPosition();
    glTranslatef(position.x + 0.5, position.y + 0.7, position.z + 0.5);
    glScalef(0.5f, 0.5f, 0.5f);
    DrawOBJ(appleModel, appleTexture);
    glPopMatrix();
}

Apple::Apple(Vector3 coordinates, const char *objPath, const char *texPath) : GameObject(coordinates)
{
    glShadeModel(GL_SMOOTH);
    LoadOBJ(objPath, appleModel);
    appleTexture = LoadTexture(texPath);
    logicPos = coordinates;
    anim = new Animator(this);
    anim->SetPosition(coordinates);
    up = true;
}