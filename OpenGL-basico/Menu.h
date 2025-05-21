#pragma once
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_opengl.h>

class Menu {
public:
    Menu();
    ~Menu();

    // Inicializa la fuente
    bool initFont(const char* fontPath, int fontSize);
    void freeFont();

    // Muestra el menú y retorna true si el usuario pulsa Jugar
    bool show(SDL_Window* window);

    // Dibuja el menú en proyección ortogonal
    void drawMenu(int width, int height);
    // Dibuja texto usando SDL_ttf y OpenGL
    void drawText(const char* text, float x, float y, SDL_Color color);

private:
    TTF_Font* font;
    GLuint textTexture;
    int textWidth;
    int textHeight;
}; 