#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include "OpenGL-basico/Game.h"
#include "OpenGL-basico/Camera.h"
#include "OpenGL-basico/Colors.h"

using namespace std;

int width = 800, height = 600;
float camAngleX = 20.0f, camAngleY = 30.0f;
float radius = 5.0f;
int lastMouseX, lastMouseY;
bool isDragging = false;

int main(int argc, char* argv[]) {
    Vector3 characterPosition(0.0f, 0.0f, 0.0f);
    Camera* camera = new Camera();
    camera->Initialize(0.0f, 0.0f, 5.0f);
    camera->SetCharacterReference(&characterPosition);
    Game game(10, width, height, camAngleX, camAngleY, radius, camera, characterPosition);
    game.loadGameObjectsFromXML("level1.xml");
    game.loop();
    game.destroy();
    return 0;
    return 0;
}
