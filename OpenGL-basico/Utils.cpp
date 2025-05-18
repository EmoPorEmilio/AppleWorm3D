#include "SDL.h"
#include "SDL_opengl.h"
#include <GL/glu.h>
#include "Colors.h"
#include "Utils.h"

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