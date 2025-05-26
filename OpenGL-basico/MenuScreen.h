#pragma once
#define NOMINMAX // Definir NOMINMAX antes de incluir windows.h
#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL.h>
#include <SDL_ttf.h>
#include "Button.h"
#include <vector>

enum class MenuAction {
    None,
    Play,
    Options,
    Exit
};

class MenuScreen {
public:
    explicit MenuScreen(TTF_Font* titleFont);
    ~MenuScreen();

    // Muestra el menú principal y devuelve la acción seleccionada
    MenuAction run(SDL_Window* window);

private:
    void handleMouse(int mouseX, int mouseY);
    void handleKey(const SDL_KeyboardEvent& key);
    void renderTitle();
    void renderButtons();
    void drawButton(const UIButton& btn);

    std::vector<UIButton> buttons;
    
    // Estados (se inicializarán en el constructor)
    int hoverId;
    int focusId;

    TTF_Font* font;
    SDL_Window* window;
    int viewportWidth, viewportHeight;
}; 