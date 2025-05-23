#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include "Button.h"

class LevelSelectionScreen {
public:
    explicit LevelSelectionScreen(TTF_Font* uiFont);

    // Muestra la pantalla; devuelve true si se seleccionó un nivel y se debe iniciar el juego.
    // Devuelve false si el usuario eligió "Atrás" o salir.
    // selectedLevel devuelve 1..20 si se seleccionó nivel, 0 si atrás.
    bool run(SDL_Window* window, int& selectedLevel);

private:
    void handleMouse(int mouseX, int mouseY);
    void handleKey(const SDL_KeyboardEvent& key);
    void renderTitle(int viewportW, int viewportH);
    void renderButtons();
    void drawButton(const UIButton& btn);

    std::vector<UIButton> levelButtons;
    UIButton backButton;
    int cols{5};

    // Estados
    int hoverId{-1};
    int focusId{1};
    bool isBackButtonHover{false};

    // Ventana
    SDL_Window* window{nullptr};
    int viewportWidth{0};
    int viewportHeight{0};

    TTF_Font* font{nullptr};
}; 