#pragma once
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

class FontManager {
public:
    FontManager();
    ~FontManager();

    // Inicializar SDL_ttf
    bool init();

    // Cargar una fuente por nombre
    TTF_Font* loadFont(const std::string& path, int size);

    // Obtener una fuente previamente cargada
    TTF_Font* getFont(const std::string& path, int size);

    // Liberar todos los recursos
    void cleanup();

private:
    std::unordered_map<std::string, TTF_Font*> fonts;
    
    // Construye una clave única para el mapa a partir de path y size
    std::string makeFontKey(const std::string& path, int size);
}; 