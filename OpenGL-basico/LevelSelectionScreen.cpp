#include "LevelSelectionScreen.h"
#include <GL/glu.h>
#include <string>
#include <iostream>
#include "SDL_opengl.h" // Añadido para constantes de OpenGL vía SDL

LevelSelectionScreen::LevelSelectionScreen(TTF_Font* uiFont) : font(uiFont) {
    // Inicializar botones de nivel (20)
    levelButtons.resize(20);
    
    // La posición se calculará en run() cuando conozcamos viewport
    for (int i = 0; i < 20; ++i) {
        levelButtons[i].id = i + 1;
        levelButtons[i].text = std::to_string(i + 1);
        // Colores verdes para los botones de nivel
        levelButtons[i].colorNormal = {30, 100, 30, 255};   // Verde oscuro
        levelButtons[i].colorHover  = {50, 150, 50, 255};   // Verde claro
        levelButtons[i].borderNormal = {20, 80, 20, 255};    // Verde más oscuro para borde
        levelButtons[i].borderHover  = {70, 180, 70, 255};   // Verde más claro/brillante para borde hover
        levelButtons[i].textColor = {230, 230, 230, 255}; // Texto claro
    }
    
    backButton.id = 0; // 0 para indicar acción de volver/cancelar
    backButton.text = "Atras";
    // Colores rojos para el botón "Atrás"
    backButton.colorNormal = {139, 0, 0, 255};       // Rojo oscuro
    backButton.colorHover  = {178, 34, 34, 255};      // Rojo "firebrick"
    backButton.borderNormal = {100, 0, 0, 255};       // Borde rojo más oscuro
    backButton.borderHover  = {205, 92, 92, 255};      // Borde rojo "indian red"
    backButton.textColor = {230, 230, 230, 255};    // Texto claro
}

bool LevelSelectionScreen::run(SDL_Window* window, int& selectedLevel) {
    this->window = window;
    SDL_GetWindowSize(window, &viewportWidth, &viewportHeight);
    
    // Guardar estado GL anterior
    GLboolean depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean textureWasEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean lightingWasEnabled = glIsEnabled(GL_LIGHTING);
    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    
    // Configuración para UI 2D
    if (depthTestWasEnabled) glDisable(GL_DEPTH_TEST);
    if (lightingWasEnabled) glDisable(GL_LIGHTING);
    if (textureWasEnabled) glDisable(GL_TEXTURE_2D);
    if (!blendWasEnabled) glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configuración de proyección y matriz
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewportWidth, 0, viewportHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Inicializar UI y posicionar botones
    float boxWidth = 70.0f;  // Ancho para botones de números
    float boxHeight = 70.0f; // Alto para botones de números
    float spacing = 15.0f;
    float totalW = cols * boxWidth + (cols - 1) * spacing;
    float startX = (viewportWidth - totalW) / 2.0f;
    float startY = viewportHeight * 0.75f - boxHeight; // Ajustado para usar boxHeight
    for (int i = 0; i < 20; ++i) {
        int row = i / cols;
        int col = i % cols;
        float x = startX + col * (boxWidth + spacing);
        float y = startY - row * (boxHeight + spacing);
        levelButtons[i].rect = {static_cast<int>(x), static_cast<int>(y), 
                                  static_cast<int>(boxWidth), static_cast<int>(boxHeight)};
    }
    // Ajustar el ancho del botón "Atrás" y centrarlo
    float backButtonWidth = 200.0f; // Aumentado el ancho
    float backButtonHeight = 50.0f;
    backButton.rect = { static_cast<int>((viewportWidth - backButtonWidth) / 2.0f), 
                        static_cast<int>(startY - (20 / cols) * (boxHeight + spacing) - spacing - backButtonHeight), // Posicionarlo debajo de la última fila de niveles
                        static_cast<int>(backButtonWidth), 
                        static_cast<int>(backButtonHeight) };

    bool running = true;
    bool levelSelected = false;

    SDL_Event event;
    while (running) {
        hoverId = -1;
        int mx,my; SDL_GetMouseState(&mx,&my); my = viewportHeight - my;
        handleMouse(mx,my);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { 
                running = false;
                selectedLevel = -1; 
            }
            if (event.type == SDL_KEYDOWN) {
                handleKey(event.key);
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    selectedLevel = -1;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                if (hoverId >= 0) { 
                    selectedLevel = hoverId; 
                    levelSelected = true;
                    running = false; 
                }
                else if (isBackButtonHover) {
                    running = false;
                    selectedLevel = -1;
                }
            }
        }

        // Renderizar
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        renderButtons();
        
        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
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
    
    return levelSelected && selectedLevel > 0;
}

void LevelSelectionScreen::handleMouse(int x, int y){
    hoverId = -1;
    for(auto &b : levelButtons){
        b.hover = false;
        if(x >= b.rect.x && x <= b.rect.x + b.rect.w && 
           y >= b.rect.y && y <= b.rect.y + b.rect.h){ 
            b.hover = true; 
            hoverId = b.id; 
        }
    }
    
    isBackButtonHover = (x >= backButton.rect.x && 
                         x <= backButton.rect.x + backButton.rect.w && 
                         y >= backButton.rect.y && 
                         y <= backButton.rect.y + backButton.rect.h);
    
    backButton.hover = isBackButtonHover;
    if(isBackButtonHover) hoverId = 0;
}

void LevelSelectionScreen::handleKey(const SDL_KeyboardEvent& key){
    if(key.keysym.sym==SDLK_ESCAPE){ hoverId=0; focusId=0; return; }
    if(key.keysym.sym==SDLK_RETURN||key.keysym.sym==SDLK_SPACE){ if(focusId>=0) { hoverId=focusId; return; } }
    // Navegación simple: izquierda/derecha
    if(key.keysym.sym==SDLK_LEFT){ if(focusId>1) focusId--; }
    if(key.keysym.sym==SDLK_RIGHT){ if(focusId>=1&&focusId<20) focusId++; }
}

void LevelSelectionScreen::renderButtons(){
    for(auto &b:levelButtons){
        b.focus = (b.id==focusId);
        drawButton(b);
    }
    backButton.focus = (focusId==0);
    drawButton(backButton);
}

void LevelSelectionScreen::drawButton(const UIButton& btn){
    SDL_Color bg = btn.hover||btn.focus? btn.colorHover: btn.colorNormal;
    SDL_Color bd = btn.hover||btn.focus? btn.borderHover: btn.borderNormal;
    glColor3ub(bd.r,bd.g,bd.b);
    glRectf(btn.rect.x-2,btn.rect.y-2, btn.rect.x+btn.rect.w+2, btn.rect.y+btn.rect.h+2);
    glColor3ub(bg.r,bg.g,bg.b);
    glRectf(btn.rect.x,btn.rect.y, btn.rect.x+btn.rect.w, btn.rect.y+btn.rect.h);
    if(!font) return;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, btn.text.c_str(), btn.textColor);
    
    // Intercambiamos los canales rojo y azul manualmente
    Uint32* pixels = static_cast<Uint32*>(surf->pixels);
    for (int i = 0; i < surf->w * surf->h; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], surf->format, &r, &g, &b, &a);
        pixels[i] = SDL_MapRGBA(surf->format, b, g, r, a); // Intercambiar R y B
    }
    
    GLuint tex; glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surf->w,surf->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surf->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    int w=surf->w,h=surf->h; SDL_FreeSurface(surf);
    glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    float tx = btn.rect.x + (btn.rect.w - w)/2.0f;
    float ty = btn.rect.y + (btn.rect.h - h)/2.0f;
    glBegin(GL_QUADS);
        glTexCoord2f(0,1); glVertex2f(tx,ty);
        glTexCoord2f(1,1); glVertex2f(tx+w,ty);
        glTexCoord2f(1,0); glVertex2f(tx+w,ty+h);
        glTexCoord2f(0,0); glVertex2f(tx,ty+h);
    glEnd();
    glDisable(GL_BLEND); glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1,&tex);
} 