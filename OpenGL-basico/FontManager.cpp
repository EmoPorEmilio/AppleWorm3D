#include "FontManager.h"
#include <iostream>

FontManager::FontManager() {
    // Constructor, la inicialización se hace en init()
}

FontManager::~FontManager() {
    cleanup();
}

bool FontManager::init() {
    // TTF_Init() ya se llama en main.cpp. 
    // Esta función se mantiene por compatibilidad o podría eliminarse.
    return true;
}

TTF_Font* FontManager::loadFont(const std::string& path, int size) {
    std::string key = makeFontKey(path, size);
    
    // Comprobar si ya está cargada
    auto it = fonts.find(key);
    if (it != fonts.end()) {
        return it->second;
    }
    
    // Cargar nueva fuente
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if (!font) {
        std::cerr << "Error al cargar fuente " << path << ": " << TTF_GetError() << std::endl;
        return nullptr;
    }
    
    // Guardar en la caché
    fonts[key] = font;
    return font;
}

TTF_Font* FontManager::getFont(const std::string& path, int size) {
    std::string key = makeFontKey(path, size);
    
    auto it = fonts.find(key);
    if (it != fonts.end()) {
        return it->second;
    }
    
    // Si no se encuentra, intentar cargarla
    return loadFont(path, size);
}

void FontManager::cleanup() {
    for (auto& pair : fonts) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
            pair.second = nullptr;
        }
    }
    fonts.clear();
    
    // No cerramos TTF_Quit() aquí, ya que podría ser usado por otras partes
    // del programa después de que esta instancia se destruya
}

std::string FontManager::makeFontKey(const std::string& path, int size) {
    return path + "#" + std::to_string(size);
} 