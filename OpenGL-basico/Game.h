#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>
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
    YOU_LOSE,
    ANIMATING,
    PAUSED
};

enum class GameLoopResult {
    ContinueLoop,
    GameEnded,
    GoToMainMenu,
    ExitApplication
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
    float degrees;
    bool rotate;
    Worm* worm;
    std::vector<GameObject*> gameObjects;
	Camera* camera;
    SDL_Window* window;
	SDL_GLContext glctx;
    CubeGrid* grid;
    Vector3 characterPosition;
	GameState gameState;
    GameState previousState;
    Uint64 currentTimestamp;
    Uint64 lastTimestamp;
    float gameSpeed;

    int hudDisplayNumber;
    TTF_Font* gameFont;
    SDL_Color textColor = { 255, 255, 255, 255 };
    GLuint textTextureCache;
    int textWidthCache, textHeightCache;
    bool textured;
    bool wireframe;

    void renderHUD();
    void renderPauseMenu();
    void drawTextGame(const char* text, float x, float y, SDL_Color color, bool useCache = false);
    void clearTextCache();

public:
    Game(SDL_Window* existingWindow, SDL_GLContext existingContext, TTF_Font* font, 
         int gridSize, int winWidth, int winHeight, float camAngleX, float camAngleY, float radius, Camera* camera, Vector3 characterPosition);
    ~Game();
    
    GameLoopResult loop();
    void destroy();
    
    void render(int renderWidth, int renderHeight);
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
    void animateWorm(float deltaTime);
    void updateWormReferences();

    void setHUDNumber(int number);
    int getHUDNumber() const;
    void toggleWireframe();
    void toggleTextures();
};