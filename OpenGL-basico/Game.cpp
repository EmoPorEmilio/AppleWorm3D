#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "Game.h"
#include "Colors.h"
#include "WormPart.h"
#include "Worm.h"
#include "tinyxml/tinyxml.h"
#include "Utils.h"

void Game::loadGameObjectsFromXML(const char* filename) {
    Worm* worm = nullptr;
    TiXmlDocument doc(filename);

    if (doc.LoadFile()) {
        TiXmlElement* element = doc.FirstChildElement();
        while (element != nullptr)
        {
            std::string text = element->GetText() ? element->GetText() : "No text";
            auto value = element->Value();
            std::cout << "Element Name: " << value << std::endl;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            element->QueryFloatAttribute("x", &x);
            element->QueryFloatAttribute("y", &y);
            element->QueryFloatAttribute("z", &z);

            std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;

            if (strcmp(value, "Worm") == 0) {
                TiXmlElement* current_element = element->FirstChildElement();
                Coordinates3D head(0, 0, 0), body(0, 0, 0), tail(0, 0, 0), orientation(0, 0, 0);
                while (current_element != nullptr) {
                    auto value = current_element->Value();
                    if (strcmp(value, "Head") == 0) {
                        element->QueryFloatAttribute("x", &x);
                        element->QueryFloatAttribute("y", &y);
                        element->QueryFloatAttribute("z", &z);
                        head = Coordinates3D(x, y, z);
                    }
                    else if (strcmp(value, "Body") == 0) {
                        element->QueryFloatAttribute("x", &x);
                        element->QueryFloatAttribute("y", &y);
                        element->QueryFloatAttribute("z", &z);
                        body = Coordinates3D(x, y, z);
                    }
                    else if (strcmp(value, "Tail") == 0) {
                        element->QueryFloatAttribute("x", &x);
                        element->QueryFloatAttribute("y", &y);
                        element->QueryFloatAttribute("z", &z);
                        tail = Coordinates3D(x, y, z);
                    }
                    std::cout << "Child Element Name: " << value << ", Content: " << text << std::endl;
                    current_element = current_element->NextSiblingElement();
                }
                worm = new Worm(head, body, tail, orientation);

            }
            else if (value == "Terrain") {

            }
            else if (value == "Apple") {

            }

            else if (value == "Portal") {

            }

            element = element->NextSiblingElement();
        }
    }
    else {
        std::cerr << "Failed to load XML file: " << doc.ErrorDesc() << std::endl;
    }
}

void Game::setupLighting() {
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

void Game::drawAxis(void)
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

void Game::render(int width, int height) {
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
    drawCube(0, 1, 2, 1.0, Colors::RED);
    //first row
    drawCube(0, 0, 0, 1.0, Colors::BROWN);
    drawCube(1, 0, 0, 1.0, Colors::BROWN);
    drawCube(2, 0, 0, 1.0, Colors::BROWN);
    drawCube(3, 0, 0, 1.0, Colors::BROWN);
    drawCube(4, 0, 0, 1.0, Colors::BROWN);
    drawCube(5, 0, 0, 1.0, Colors::BROWN);
    //second row
    drawCube(0, 0, 1, 1.0, Colors::BROWN);
    drawCube(1, 0, 1, 1.0, Colors::BROWN);
    drawCube(2, 0, 1, 1.0, Colors::BROWN);
    drawCube(3, 0, 1, 1.0, Colors::BROWN);
    drawCube(4, 0, 1, 1.0, Colors::BROWN);
    drawCube(5, 0, 1, 1.0, Colors::BROWN);
    //third row
    drawCube(0, 0, 2, 1.0, Colors::BROWN);
    drawCube(1, 0, 2, 1.0, Colors::BROWN);
    drawCube(2, 0, 2, 1.0, Colors::BROWN);
    drawCube(3, 0, 2, 1.0, Colors::BROWN);
    drawCube(4, 0, 2, 1.0, Colors::BROWN);
    drawCube(5, 0, 2, 1.0, Colors::BROWN);

    //worm and apple
    drawCube(2, 1, 0, 1.0, Colors::GREEN);
    drawCube(1, 1, 0, 1.0, Colors::GREEN);
    drawCube(0, 1, 0, 1.0, Colors::GREEN);
    drawCube(5, 1, 0, 1.0, Colors::BLUE);
}

Game::Game(int gridSize, int width, int height, float camAngleX, float camAngleY, float radius)
    : score(0), step(0), width(width), height(height), camAngleX(camAngleX), camAngleY(camAngleY), radius(radius), lastMouseX(0), lastMouseY(0), isDragging(false), grid(gridSize), window(nullptr)
{
    grid.init();
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "SDL2 + OpenGL 2.1 Cube",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    glctx = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
}

void Game::loop() {
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
}

void Game::destroy() {
    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}