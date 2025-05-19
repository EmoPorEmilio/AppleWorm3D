#pragma once
#include "CubeGrid.h"
#include "Colors.h"
#include "Camera.h"
#include "GameObject.h"
#include "Worm.h"
#include <vector>

enum GameState {
    WAITING_FOR_INPUT,
    PROCESSING,
    FALLING,
    YOU_WIN,
    YOU_LOSE
};

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
    Worm* worm;
    std::vector<GameObject*> gameObjects;
	Camera* camera;
    SDL_Window* window;
	SDL_GLContext glctx;
    CubeGrid* grid;
    Vector3 characterPosition;
	GameState gameState;

public:
    Game(int gridSize, int width, int height, float camAngleX, float camAngleY, float radius, Camera* camera, Vector3 characterPosition);
    void loop();
    void destroy();
    void render(int width, int height);
    void setupLighting();
    void drawAxis();
	void loadGameObjectsFromXML(const char* filename);
    void processKey(const SDL_Event& event);
    void removeWormFromGameObjectsAndCubeGrid();
    void removeGameObjectFromGameObjectsAndCubeGrid(GameObject* go, bool isWorm);
    void addGameObjectToGameObjectsAndCubeGrid(GameObject* go);
	void addWormToGameObjectsAndCubeGrid();
    void growWorm();
    bool canWormMoveForward(Vector3 orientation);
    bool willWormEatApple(Vector3 orientation);
    void eatApple();
    void fallWorm();
    void updateWormFallInCubeGrid();
    bool isWormSupported();
};