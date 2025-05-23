#include "Menu.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <cstring>
#include <SDL_ttf.h>
#include <iostream> // Para std::cerr y std::endl

Menu::Menu() : font(nullptr), textTexture(0), textWidth(0), textHeight(0) {}
Menu::~Menu() { freeFont(); }

bool Menu::initFont(const char* fontPath, int fontSize) {
    if (font) freeFont();
    font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        std::cerr << "Error al cargar la fuente '" << fontPath << "': " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Menu::freeFont() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (textTexture) {
        glDeleteTextures(1, &textTexture);
        textTexture = 0;
    }
}

void Menu::drawText(const char* text, float x, float y, SDL_Color color) {
    if (!font) return;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, color);
    if (!textSurface) return;

    if (textTexture) glDeleteTextures(1, &textTexture);
    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textWidth = textSurface->w;
    textHeight = textSurface->h;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnable(GL_BLEND); // Habilitar blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Configurar función de blending

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + textWidth, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + textWidth, y + textHeight);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + textHeight);
    glEnd();
    
    glDisable(GL_BLEND); // Deshabilitar blending
    glDisable(GL_TEXTURE_2D);

    SDL_FreeSurface(textSurface);
}

void Menu::drawMenu(int width, int height) {
    // Cambia a proyección ortogonal
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<GLdouble>(width), 0.0, static_cast<GLdouble>(height)); 
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // El fondo ya se limpia con glClearColor en Menu::show
    // No es necesario dibujar un quad de fondo aquí si glClearColor es suficiente.
    // Si quisiéramos un efecto de gradiente o imagen de fondo, lo haríamos aquí.

    // Botón Jugar
    float buttonWidth = 600.0f; // Un poco más ancho
    float buttonHeight = 60.0f; // Un poco más alto
    float buttonX = static_cast<float>(width) / 2.0f - buttonWidth / 2.0f;
    float buttonY = static_cast<float>(height) / 2.0f - buttonHeight / 2.0f - 70.0f; // Ajuste Y

    // Sombra del botón (opcional, pero da profundidad)
    glColor4f(0.0f, 0.0f, 0.0f, 0.4f); // Sombra negra semi-transparente
    glRectf(buttonX + 5, buttonY - 5, buttonX + buttonWidth + 5, buttonY + buttonHeight - 5);

    // Borde del botón
    glColor3f(0.5f, 0.0f, 0.0f); // Rojo oscuro para el borde
    glRectf(buttonX - 2, buttonY - 2, buttonX + buttonWidth + 2, buttonY + buttonHeight + 2);

    // Interior del botón
    glColor3f(0.8f, 0.1f, 0.1f); // Rojo principal para el interior del botón
    glRectf(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Texto
    SDL_Color titleColor = {50, 255, 50, 255}; // Verde neón para el título
    SDL_Color buttonTextColor = {255, 255, 200, 255}; // Blanco amarillento para el texto del botón

    // Título del juego
    float titleTextWidthEstimate = 300.0f; // Ajustar según fuente y tamaño real
    drawText("APPLE WORM 3D", static_cast<float>(width)/2.0f - titleTextWidthEstimate/2.0f, static_cast<float>(height) * 0.75f, titleColor);

    // Texto del botón "JUGAR"
    float playTextWidthEstimate = 100.0f; // Ajustar
    float playTextX = buttonX + (buttonWidth - playTextWidthEstimate) / 2.0f;
    // Para centrar verticalmente el texto en el botón, estimación:
    float playTextY = buttonY + (buttonHeight / 2.0f) - 18.0f; // Ajustar el -18.0f según el tamaño de fuente 48
    drawText("JUGAR", playTextX, playTextY, buttonTextColor);
    
    // Pequeño texto de "instrucción" o "versión" (opcional)
    SDL_Color instructionColor = {150, 150, 150, 255}; // Gris
    drawText("Presiona cualquier tecla para iniciar", static_cast<float>(width)/2.0f - 220.0f, static_cast<float>(height) * 0.1f, instructionColor);

    // Restaurar proyección
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

bool Menu::show(SDL_Window* window) {
    int width = 1280, height = 720;
    SDL_GetWindowSize(window, &width, &height);
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                // Si el usuario presiona una tecla o hace clic, salir del menú
                return true;
            }
        }
        glClearColor(0.0f, 0.1f, 0.05f, 1.0f); // Fondo verde muy oscuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawMenu(width, height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }
    return false;
} 