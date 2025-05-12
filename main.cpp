#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "OpenGL-basico/Game.h"
#include "OpenGL-basico/tinyxml/tinyxml.h"


using namespace std;

// Camera angles and distance
float camAngleX = 20.0f, camAngleY = 30.0f;
float radius = 5.0f;
int lastMouseX, lastMouseY;
bool isDragging = false;

void setupLighting() {
    //GLfloat light_pos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    //GLfloat light_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    //GLfloat light_spe[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);
}

enum Color_Material {
    GREEN,
    BROWN,
    BLUE,
    RED
};

void drawCube(int x, int y, int z, float size, Color_Material color) {
    // Material
    const GLfloat color_table[][4] = {
        {0.0f, 1.0f, 0.0f, 1.0f}, // GREEN
        {0.6f, 0.3f, 0.0f, 1.0f}, // BROWN
        {0.0f, 0.0f, 1.0f, 1.0f}, // BLUE
        {1.0f, 0.0f, 0.0f, 1.0f}  // RED
    };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, color_table[color]);

    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.5);
    glTranslatef(float(x), float(y), float(z));
    glScalef(size, size, size);

    glBegin(GL_QUADS);

    // Front
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    // Back
    glNormal3f(0, 0, -1);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    // Left
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    // Right
    glNormal3f(1, 0, 0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    // Top
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    // Bottom
    glNormal3f(0, -1, 0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    glPopMatrix();
}

void drawAxis(void)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(-50, 0, 0);
    glVertex3f(50, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, -50, 0);
    glVertex3f(0, 50, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -50);
    glVertex3f(0, 0, 50);
    glEnd();
    glEnable(GL_LIGHTING);
}

void render(int width, int height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 100.0);

    // Modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera orbit
    float camX = radius * sinf(camAngleY * 3.14159f / 180.0f) *
        cosf(camAngleX * 3.14159f / 180.0f);
    float camY = radius * sinf(camAngleX * 3.14159f / 180.0f);
    float camZ = radius * cosf(camAngleY * 3.14159f / 180.0f) *
        cosf(camAngleX * 3.14159f / 180.0f);

    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);

    setupLighting();

    drawAxis();
    drawCube(0, 1, 2, 1.0, Color_Material::RED);
    //first row
    drawCube(0, 0, 0, 1.0, Color_Material::BROWN);
    drawCube(1, 0, 0, 1.0, Color_Material::BROWN);
    drawCube(2, 0, 0, 1.0, Color_Material::BROWN);
    drawCube(3, 0, 0, 1.0, Color_Material::BROWN);
    drawCube(4, 0, 0, 1.0, Color_Material::BROWN);
    drawCube(5, 0, 0, 1.0, Color_Material::BROWN);
    //second row
    drawCube(0, 0, 1, 1.0, Color_Material::BROWN);
    drawCube(1, 0, 1, 1.0, Color_Material::BROWN);
    drawCube(2, 0, 1, 1.0, Color_Material::BROWN);
    drawCube(3, 0, 1, 1.0, Color_Material::BROWN);
    drawCube(4, 0, 1, 1.0, Color_Material::BROWN);
    drawCube(5, 0, 1, 1.0, Color_Material::BROWN);
    //third row
    drawCube(0, 0, 2, 1.0, Color_Material::BROWN);
    drawCube(1, 0, 2, 1.0, Color_Material::BROWN);
    drawCube(2, 0, 2, 1.0, Color_Material::BROWN);
    drawCube(3, 0, 2, 1.0, Color_Material::BROWN);
    drawCube(4, 0, 2, 1.0, Color_Material::BROWN);
    drawCube(5, 0, 2, 1.0, Color_Material::BROWN);

    //worm and apple
    drawCube(2, 1, 0, 1.0, Color_Material::GREEN);
    drawCube(1, 1, 0, 1.0, Color_Material::GREEN);
    drawCube(0, 1, 0, 1.0, Color_Material::GREEN);
    drawCube(5, 1, 0, 1.0, Color_Material::BLUE);
}

int main(int argc, char* argv[]) {
	TiXmlDocument doc("level1.xml");

    if (doc.LoadFile()){
        TiXmlElement* element = doc.FirstChildElement();
        while (element != nullptr)
        {
            std::string text = element->GetText() ? element->GetText() : "No text";
            std::cout << "Element Name: " << element->Value() << ", Content: " << text << std::endl;

           float x = 0.0f, y = 0.0f, z = 0.0f;
           element->QueryFloatAttribute("x", &x);
           element->QueryFloatAttribute("y", &y);
           element->QueryFloatAttribute("z", &z);

           std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

           element = element->NextSiblingElement();
        }
    }
    else {
    std::cerr << "Failed to load XML file: " << doc.ErrorDesc() << std::endl;
    }

	Game game(10);
    game.init();

    SDL_Init(SDL_INIT_VIDEO);

    // Request OpenGL 2.1 context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int width = 800, height = 600;
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 + OpenGL 2.1 Cube",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    SDL_GLContext glctx = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    lastMouseX = event.button.x;
                    lastMouseY = event.button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (isDragging) {
                    int dx = event.motion.x - lastMouseX;
                    int dy = event.motion.y - lastMouseY;
                    camAngleY += dx;
                    camAngleX += dy;
                    if (camAngleX > 89) camAngleX = 89;
                    if (camAngleX < -89) camAngleX = -89;
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                }
                break;
            case SDL_MOUSEWHEEL:
                // Zoom in/out
                radius -= event.wheel.y * 0.5f; // y > 0 is scroll up
                if (radius < 2.0f) radius = 2.0f;
                if (radius > 20.0f) radius = 20.0f;
                break;
            }
        }

        render(width, height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
