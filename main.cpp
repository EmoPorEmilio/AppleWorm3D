#ifdef _WIN32
#include <windows.h>
#endif
#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include "OpenGL-basico/Game.h"
#include "OpenGL-basico/Camera.h"
#include "OpenGL-basico/Colors.h"
#include "OpenGL-basico/FontManager.h"
#include "OpenGL-basico/MenuScreen.h"
#include "OpenGL-basico/LevelSelectionScreen.h"
#include <string>
#include <direct.h> // Para _getcwd
#include <SDL_ttf.h> // Asegurarse de incluir para TTF_Init, etc.

using namespace std;

int main(int argc, char* argv[]) {
    // Imprimir directorio de trabajo actual
    char cwdBuffer[FILENAME_MAX];
    _getcwd(cwdBuffer, FILENAME_MAX);
    std::cout << "Directorio de trabajo actual: " << cwdBuffer << std::endl;

    // Inicializaciones globales
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error al iniciar SDL Video: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "Error al iniciar SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    FreeImage_Initialise();

    const int windowWidth = 1280;
    const int windowHeight = 720;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* gameWindow = SDL_CreateWindow("Apple Worm 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!gameWindow) {
        std::cerr << "No se pudo crear la ventana SDL: " << SDL_GetError() << std::endl;
        FreeImage_DeInitialise(); TTF_Quit(); SDL_Quit();
        return -1;
    }

    SDL_GLContext gameCtx = SDL_GL_CreateContext(gameWindow);
    if (!gameCtx) {
        std::cerr << "No se pudo crear el contexto GL: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gameWindow); FreeImage_DeInitialise(); TTF_Quit(); SDL_Quit();
        return -1;
    }

    FontManager fontManager;
    // fontManager.init(); // Ya no es necesaria si TTF_Init se llama aquí

    TTF_Font* uiFont = fontManager.loadFont("fuente.TTF", 24); // Tamaño más pequeño para UI general y HUD
    TTF_Font* titleFont = fontManager.loadFont("fuente.TTF", 48); // Tamaño más grande para títulos

    if (!uiFont || !titleFont) {
        std::cerr << "Error crítico: No se pudieron cargar las fuentes necesarias (fuente.TTF)." << std::endl;
        // Considerar cerrar si las fuentes son vitales
        fontManager.cleanup();
        SDL_GL_DeleteContext(gameCtx);
        SDL_DestroyWindow(gameWindow);
        FreeImage_DeInitialise();
        TTF_Quit();
        SDL_Quit();
        return -1; 
    }

    bool applicationRunning = true;
    bool displayingMainMenu = true; // Cambiado para más claridad
    int currentLevel = 1;   

    while(applicationRunning){
        if (displayingMainMenu) {
            MenuScreen menuScreen(titleFont); 
            MenuAction menuAction = menuScreen.run(gameWindow);
            
            if (menuAction == MenuAction::Play) {
                LevelSelectionScreen levelSelector(uiFont);
                bool levelWasSelected = levelSelector.run(gameWindow, currentLevel);
                if (levelWasSelected && currentLevel > 0) {
                    displayingMainMenu = false; // Listo para jugar
                } else {
                    applicationRunning = false; // Salió/canceló de LevelSelectionScreen
                }
            } else { // Exit o cerró ventana de MenuScreen
                applicationRunning = false;
            }
        } // Fin if (displayingMainMenu)

        // Si salimos de los menús y todavía estamos corriendo la aplicación, entramos al juego
        if (applicationRunning && !displayingMainMenu) {
    Vector3 characterPosition(15.0f, 11.0f, 10.0f);
    Camera* camera = new Camera();
    camera->Initialize(0.0f, 0.0f, 5.0f);
    camera->SetCharacterReference(&characterPosition);

            Game game(gameWindow, gameCtx, uiFont, 10, windowWidth, windowHeight, 50.0f, 30.0f, 30.0f, camera, characterPosition);
            
            std::string levelFile = "level" + std::to_string(currentLevel) + ".xml";
            std::cout << "Cargando nivel: " << levelFile << std::endl;
            game.loadGameObjectsFromXML(levelFile.c_str());
            
            GameLoopResult gameResult = game.loop();
            
    game.destroy();
            delete camera;  

            // Limpiar estado OpenGL después de salir del juego
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_BLEND);
            
            // Restablecer viewport y matrices
            glViewport(0, 0, windowWidth, windowHeight);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            // Limpia los buffers para el próximo ciclo
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SDL_GL_SwapWindow(gameWindow);

            if (gameResult == GameLoopResult::GoToMainMenu) {
                displayingMainMenu = true; // Para la siguiente iteración del while, mostrará el menú
                // No necesitamos 'continue' aquí, el flujo natural del while lo hará
            } else if (gameResult == GameLoopResult::ExitApplication) {
                applicationRunning = false; 
            } else if (gameResult == GameLoopResult::GameEnded) {
                displayingMainMenu = true; // Volver al menú después de que el juego termine
            }
        } 
    } // Fin while(applicationRunning)

    fontManager.cleanup();
    SDL_GL_DeleteContext(gameCtx);
    SDL_DestroyWindow(gameWindow);
    FreeImage_DeInitialise();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
