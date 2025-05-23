#pragma once
#include <SDL.h>
#include <string>

struct UIButton {
    SDL_Rect rect{0,0,0,0};
    std::string text;
    int id{0};               // ID del botón. 0 para "Atrás", 1..N para niveles

    bool hover{false};       // Estado de hover por ratón
    bool focus{false};       // Estado de selección por teclado

    SDL_Color colorNormal{50,50,150,255};
    SDL_Color colorHover{100,100,200,255};
    SDL_Color colorFocus{120,120,255,255};  // Si lo selecciona el teclado

    SDL_Color borderNormal{100,100,200,255};
    SDL_Color borderHover{150,150,255,255};
    SDL_Color borderFocus{180,180,255,255};

    SDL_Color textColor{255,255,255,255};
}; 