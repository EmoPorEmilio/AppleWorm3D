#include "MenuScreen.h"
#include "SDL_opengl.h" // Añadido para constantes y funciones de OpenGL vía SDL
#include <GL/glu.h> // Para gluOrtho2D
#include <iostream> // Para std::cout, std::cerr

// Constructor
MenuScreen::MenuScreen(TTF_Font* titleFont) : font(titleFont), window(nullptr), viewportWidth(0), viewportHeight(0), hoverId(-1), focusId(0) {
    // Inicializar botones
    buttons.resize(2); // Play, Exit

    buttons[0].id = static_cast<int>(MenuAction::Play);
    buttons[0].text = "Jugar";
    buttons[0].colorNormal = {30, 100, 30, 255};
    buttons[0].colorHover  = {50, 150, 50, 255};
    buttons[0].borderNormal = {20, 80, 20, 255};
    buttons[0].borderHover  = {70, 180, 70, 255};
    buttons[0].textColor = {230, 230, 230, 255};

    buttons[1].id = static_cast<int>(MenuAction::Exit);
    buttons[1].text = "Salir";
    buttons[1].colorNormal = {139, 0, 0, 255};
    buttons[1].colorHover  = {178, 34, 34, 255};
    buttons[1].borderNormal = {100, 0, 0, 255};
    buttons[1].borderHover  = {205, 92, 92, 255};
    buttons[1].textColor = {230, 230, 230, 255};

    // Las posiciones se calcularán en run()
    std::cout << "MenuScreen construido." << std::endl;
}

// Destructor
MenuScreen::~MenuScreen() {
    // No hay recursos que liberar aquí directamente, la fuente es manejada por FontManager
    // y la ventana por SDL en main.
    std::cout << "MenuScreen destruido." << std::endl;
}

// Bucle principal del menú
MenuAction MenuScreen::run(SDL_Window* w) {
    this->window = w;
    SDL_GetWindowSize(window, &viewportWidth, &viewportHeight);

    // Calcular posiciones de los botones
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float spacing = 20.0f;
    float totalButtonHeight = buttons.size() * buttonHeight + (buttons.size() - 1) * spacing;
    float startY = viewportHeight / 2.0f + totalButtonHeight / 2.0f - buttonHeight; // Centrar verticalmente

    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].rect = {
            static_cast<int>((viewportWidth - buttonWidth) / 2.0f),
            static_cast<int>(startY - i * (buttonHeight + spacing)),
            static_cast<int>(buttonWidth),
            static_cast<int>(buttonHeight)
        };
    }


    // Guardar estado GL anterior
    GLboolean depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean textureWasEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean lightingWasEnabled = glIsEnabled(GL_LIGHTING);
    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);

    // Configuración para UI 2D
    if (depthTestWasEnabled) glDisable(GL_DEPTH_TEST);
    if (lightingWasEnabled) glDisable(GL_LIGHTING);
    if (textureWasEnabled) glDisable(GL_TEXTURE_2D); // Se habilitará al dibujar texto
    if (!blendWasEnabled) glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewportWidth, 0, viewportHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    bool running = true;
    MenuAction selectedAction = MenuAction::None;
    SDL_Event event;

    while (running) {
        hoverId = -1; // Reset hover
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        my = viewportHeight - my; // Coordenadas de SDL a OpenGL
        handleMouse(mx, my);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                selectedAction = MenuAction::Exit;
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    selectedAction = MenuAction::Exit; // O podría ser MenuAction::Back si tuviéramos submenús
                    running = false;
                }
                handleKey(event.key); // Para navegación con teclado si se implementa
                 if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) {
                    if (focusId != -1) { // Asumiendo que focusId se actualiza correctamente
                         for(const auto& btn : buttons){
                            if(btn.id == focusId){ // O si focusId es el índice
                                selectedAction = static_cast<MenuAction>(btn.id);
                                running = false;
                                break;
                            }
                        }
                        if (static_cast<size_t>(focusId) < buttons.size()) { // Si focusId es un índice
                             selectedAction = static_cast<MenuAction>(buttons[focusId].id);
                             running = false;
                        }
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT && hoverId != -1) {
                    selectedAction = static_cast<MenuAction>(hoverId);
                    running = false;
                }
            }
        }

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Fondo gris oscuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        renderTitle();
        renderButtons();

        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }

    // Restaurar estado GL
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    if (depthTestWasEnabled) glEnable(GL_DEPTH_TEST);
    if (lightingWasEnabled) glEnable(GL_LIGHTING);
    if (textureWasEnabled) glEnable(GL_TEXTURE_2D);
    if (!blendWasEnabled) glDisable(GL_BLEND);
    
    return selectedAction;
}

void MenuScreen::handleMouse(int mouseX, int mouseY) {
    hoverId = -1; // Por defecto, ningún botón está hovereado
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].hover = false;
        if (mouseX >= buttons[i].rect.x && mouseX <= buttons[i].rect.x + buttons[i].rect.w &&
            mouseY >= buttons[i].rect.y && mouseY <= buttons[i].rect.y + buttons[i].rect.h) {
            buttons[i].hover = true;
            hoverId = buttons[i].id;
        }
    }
}

void MenuScreen::handleKey(const SDL_KeyboardEvent& key) {
    if (buttons.empty()) return;

    if (key.keysym.sym == SDLK_UP) {
        if (focusId == -1 && !buttons.empty()) { // Si no hay foco, selecciona el último
             focusId = buttons.back().id;
        } else {
            // Encontrar el índice actual del botón con focusId
            int currentIndex = -1;
            for(size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].id == focusId) {
                    currentIndex = i;
                    break;
                }
            }
            if (currentIndex != -1) {
                currentIndex = (currentIndex - 1 + buttons.size()) % buttons.size();
                focusId = buttons[currentIndex].id;
            } else if (!buttons.empty()) { // Si focusId no era válido, empieza por el primero
                 focusId = buttons[0].id;
            }
        }
    } else if (key.keysym.sym == SDLK_DOWN) {
         if (focusId == -1 && !buttons.empty()) { // Si no hay foco, selecciona el primero
             focusId = buttons.front().id;
        } else {
            int currentIndex = -1;
            for(size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].id == focusId) {
                    currentIndex = i;
                    break;
                }
            }
            if (currentIndex != -1) {
                currentIndex = (currentIndex + 1) % buttons.size();
                focusId = buttons[currentIndex].id;
            } else if (!buttons.empty()) { // Si focusId no era válido, empieza por el primero
                 focusId = buttons[0].id;
            }
        }
    }
    // Actualizar el estado de 'focus' de los botones
    for(auto& btn : buttons) {
        btn.focus = (btn.id == focusId);
    }
}


void MenuScreen::renderTitle() {
    if (!font) return;

    const char* titleText = "Apple Worm 3D";
    SDL_Color titleColor = {200, 200, 255, 255}; // Azul claro

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, titleText, titleColor);
    if (!surface) {
        std::cerr << "Error al renderizar texto del título: " << TTF_GetError() << std::endl;
        return;
    }

    // Intercambiamos los canales rojo y azul manualmente
    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    for (int i = 0; i < surface->w * surface->h; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], surface->format, &r, &g, &b, &a);
        pixels[i] = SDL_MapRGBA(surface->format, b, g, r, a); // Intercambiar R y B
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    float textWidth = static_cast<float>(surface->w);
    float textHeight = static_cast<float>(surface->h);
    SDL_FreeSurface(surface);

    float posX = (viewportWidth - textWidth) / 2.0f;
    float posY = viewportHeight * 0.8f; // Parte superior de la pantalla

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); // Asegurarse que el blend esté habilitado para transparencia
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Color blanco para no teñir la textura

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(posX, posY - textHeight); // SDL usualmente tiene (0,0) arriba-izquierda, OpenGL abajo-izquierda
        glTexCoord2f(1, 1); glVertex2f(posX + textWidth, posY - textHeight);
        glTexCoord2f(1, 0); glVertex2f(posX + textWidth, posY);
        glTexCoord2f(0, 0); glVertex2f(posX, posY);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND); // Deshabilitar blend si no se necesita más
    glDeleteTextures(1, &textureID);
}

void MenuScreen::renderButtons() {
    for (const auto& btn : buttons) {
        drawButton(btn);
    }
}

void MenuScreen::drawButton(const UIButton& btn) {
    SDL_Color bgColor = (btn.hover || btn.focus) ? btn.colorHover : btn.colorNormal;
    SDL_Color borderColor = (btn.hover || btn.focus) ? btn.borderHover : btn.borderNormal;

    // Dibujar borde
    glColor3ub(borderColor.r, borderColor.g, borderColor.b);
    glRectf(static_cast<float>(btn.rect.x - 2), static_cast<float>(btn.rect.y - 2),
            static_cast<float>(btn.rect.x + btn.rect.w + 2), static_cast<float>(btn.rect.y + btn.rect.h + 2));

    // Dibujar fondo
    glColor3ub(bgColor.r, bgColor.g, bgColor.b);
    glRectf(static_cast<float>(btn.rect.x), static_cast<float>(btn.rect.y),
            static_cast<float>(btn.rect.x + btn.rect.w), static_cast<float>(btn.rect.y + btn.rect.h));
    
    // Dibujar texto del botón
    if (!font || btn.text.empty()) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, btn.text.c_str(), btn.textColor);
    if (!surface) {
        std::cerr << "Error al renderizar texto del botón '" << btn.text << "': " << TTF_GetError() << std::endl;
        return;
    }

    // Intercambiamos los canales rojo y azul manualmente
    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    for (int i = 0; i < surface->w * surface->h; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], surface->format, &r, &g, &b, &a);
        pixels[i] = SDL_MapRGBA(surface->format, b, g, r, a); // Intercambiar R y B
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = GL_RGBA;
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

    float textWidth = static_cast<float>(surface->w);
    float textHeight = static_cast<float>(surface->h);

    // Centrar texto en el botón
    float textPosX = btn.rect.x + (btn.rect.w - textWidth) / 2.0f;
    float textPosY = btn.rect.y + (btn.rect.h - textHeight) / 2.0f;
    
    SDL_FreeSurface(surface);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); // Asegurarse que el blend esté habilitado
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Textura sin teñir

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(textPosX, textPosY); // Y=1 arriba
        glTexCoord2f(1, 1); glVertex2f(textPosX + textWidth, textPosY);
        glTexCoord2f(1, 0); glVertex2f(textPosX + textWidth, textPosY + textHeight);
        glTexCoord2f(0, 0); glVertex2f(textPosX, textPosY + textHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    // No deshabilitar BLEND aquí si el título también lo usa y se renderiza después, o viceversa.
    // Se gestiona globalmente en run()
    glDeleteTextures(1, &textureID);
}
