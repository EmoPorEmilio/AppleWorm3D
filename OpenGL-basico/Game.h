#pragma once
#include "CubeGrid.h"
#include "Colors.h"

class Game {
private:
    int score;
    int step;
	int width;
	int height;
    float camAngleX;
    float camAngleY;
    float radius;
    int lastMouseX;
    int lastMouseY;
    bool isDragging;
    SDL_Window* window;
	SDL_GLContext glctx;
    CubeGrid grid;

public:
    Game(int gridSize, int width, int height, float camAngleX, float camAngleY, float radius);
    void loop();
    void destroy();
    void render(int width, int height);
    void setupLighting();
    void drawAxis();
	void loadGameObjectsFromXML(const char* filename);
};