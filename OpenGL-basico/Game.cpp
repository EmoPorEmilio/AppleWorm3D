#include "SDL.h"
#include "SDL_opengl.h"
#include <SDL_ttf.h>
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include <string>
#include "Game.h"
#include "Colors.h"
#include "WormPart.h"
#include "Worm.h"
#include "Terrain.h"
#include "Lava.h"
#include "Apple.h"
#include "Portal.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "CubeGrid.h"
#include "tinyxml/tinyxml.h"
#include "Utils.h"

SDL_Rect resumeButtonRect;
SDL_Rect wireframeButtonRect;
SDL_Rect texturesButtonRect;
SDL_Rect shadingButtonRect;
SDL_Rect gameSpeedButtonRect;
SDL_Rect mainMenuButtonRect;
SDL_Rect exitButtonRect;

void Game::loadGameObjectsFromXML(const char* filename) {
    TiXmlDocument doc(filename);

    if (doc.LoadFile()) {
        float size = 0;
        TiXmlElement* element = doc.FirstChildElement();
        element->QueryFloatAttribute("size", &size);
        this->grid = new CubeGrid((int) size);
        element = element->FirstChildElement();
        while (element != nullptr)
        {
            auto value = element->Value();
            float x = 0.0f, y = 0.0f, z = 0.0f;
            element->QueryFloatAttribute("x", &x);
            element->QueryFloatAttribute("y", &y);
            element->QueryFloatAttribute("z", &z);

            if (strcmp(value, "Worm") == 0) {
                TiXmlElement* current_element = element->FirstChildElement();
                Vector3 head(0, 0, 0), body(0, 0, 0), tail(0, 0, 0), orientationForward(0, 0, 0), orientationUp(0, 0, 0);
                while (current_element != nullptr) {
                    auto current_tag_value = current_element->Value();
                    if (strcmp(current_tag_value, "Head") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        head = Vector3(x, y, z);
                    }
                    else if (strcmp(current_tag_value, "Body") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        body = Vector3(x, y, z);
                    }
                    else if (strcmp(current_tag_value, "Tail") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        tail = Vector3(x, y, z);
                    }
					else if (strcmp(current_tag_value, "OrientationForward") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
						orientationForward = Vector3(x, y, z);
					}
                    else if (strcmp(current_tag_value, "OrientationUp") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        orientationUp = Vector3(x, y, z);
                    }
                    current_element = current_element->NextSiblingElement();
                }
                WormPart* headPart = new WormPart(head, WormPartType::Head);
                WormPart* bodyPart = new WormPart(body, WormPartType::Body);
                WormPart* tailPart = new WormPart(tail, WormPartType::Tail);

				Worm* newWorm = new Worm(headPart, bodyPart, tailPart, orientationForward, orientationUp);
                this->grid->setObject(head, headPart);
                this->grid->setObject(body, bodyPart);
                this->grid->setObject(tail, tailPart);
                this->gameObjects.push_back(headPart);
                this->gameObjects.push_back(bodyPart);
                this->gameObjects.push_back(tailPart);
                this->worm = newWorm;
            }
            else if ((strcmp(value, "Terrain") == 0)) {
                Terrain* terrain = new Terrain(Vector3(x, y, z), "resources/terrain_cube_for_a_pl_0518160801_texture.obj", "resources/terrain_cube_for_a_pl_0518160801_texture.png");
                this->gameObjects.push_back(terrain);
				this->grid->setObject(Vector3(x, y, z), terrain);
            }
            else if ((strcmp(value, "Apple") == 0)) {
                Apple* apple = new Apple(Vector3(x, y, z), "resources/polished_apple_glossy_0518183850_texture.obj", "resources/polished_apple_glossy_0518183850_texture.png");
                this->gameObjects.push_back(apple);
                this->grid->setObject(Vector3(x, y, z), apple);
            }
            else if ((strcmp(value, "Portal") == 0)) {
                Portal* portal = new Portal(Vector3(x, y, z), "resources/circular_cosmic_porta_0518183805_texture.obj", "resources/circular_cosmic_porta_0518183805_texture.png");
                this->gameObjects.push_back(portal);
                this->grid->setObject(Vector3(x, y, z), portal);
            }
            else if ((strcmp(value, "Lava") == 0)) {
                Lava* lava = new Lava(Vector3(x, y, z), "resources/lava_cube_0518183714_texture.obj", "resources/lava_cube_0518183714_texture.png");
                this->gameObjects.push_back(lava);
                this->grid->setObject(Vector3(x, y, z), lava);
            }

            element = element->NextSiblingElement();
        }
    }
    else {
        std::cerr << "Failed to load XML file: " << filename << " - " << doc.ErrorDesc() << std::endl;
    }
}

void Game::setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    GLfloat light0_pos[] = { 15.0f, 15.0f, 12.0f, 1.0f };
    GLfloat light0_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light0_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light0_spe[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spe);

    GLfloat light1_pos[] = { -10.0f, 10.0f, 10.0f, 1.0f };
    GLfloat light1_dif[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
}

void Game::drawAxis(void)
{
    GLboolean lightingEnabled;
    glGetBooleanv(GL_LIGHTING, &lightingEnabled);
    if (lightingEnabled) glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-50.0f, 0.0f, 0.0f);
    glVertex3f(50.0f, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -50.0f, 0.0f);
    glVertex3f(0.0f, 50.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -50.0f);
    glVertex3f(0.0f, 0.0f, 50.0f);
    glEnd();

    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void Game::clearTextCache() {
    if (textTextureCache != 0) {
        glDeleteTextures(1, &textTextureCache);
        textTextureCache = 0;
    }
}

void Game::drawTextGame(const char* text, float x, float y, SDL_Color color, bool useCache) {
    if (!gameFont) return;
    if (text == nullptr || *text == '\0') return;

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(gameFont, text, color);
    if (!textSurface) {
        std::cerr << "TTF_RenderUTF8_Blended error: " << TTF_GetError() << std::endl;
        return;
    }

    bool needsNewTexture = !useCache || (useCache && textTextureCache == 0);

    if (needsNewTexture) {
        if (textTextureCache != 0) {
            glDeleteTextures(1, &textTextureCache);
            textTextureCache = 0;
        }
        glGenTextures(1, &textTextureCache); 
        glBindTexture(GL_TEXTURE_2D, textTextureCache);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0,
                     GL_BGRA, 
                     GL_UNSIGNED_BYTE, textSurface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        textWidthCache = textSurface->w;
        textHeightCache = textSurface->h;
    } else {
        glBindTexture(GL_TEXTURE_2D, textTextureCache);
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, this->width, 0, this->height); 
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLboolean lightingWasOnText;
    glGetBooleanv(GL_LIGHTING, &lightingWasOnText);
    if (lightingWasOnText) {
        glDisable(GL_LIGHTING);
    }
    GLboolean depthTestWasOnText;
    glGetBooleanv(GL_DEPTH_TEST, &depthTestWasOnText);
    if (depthTestWasOnText) {
         glDisable(GL_DEPTH_TEST);
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    float oglY = this->height - y - textHeightCache; 

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, oglY);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + textWidthCache, oglY);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + textWidthCache, oglY + textHeightCache);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, oglY + textHeightCache);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    if (depthTestWasOnText) {
         glEnable(GL_DEPTH_TEST);
    }
    if (lightingWasOnText) {
        glEnable(GL_LIGHTING);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    SDL_FreeSurface(textSurface);
}

void Game::renderHUD() {
    std::string hudText = std::to_string(hudDisplayNumber);
    float x = 10.0f;
    float y = 10.0f; 
    drawTextGame(hudText.c_str(), x, y, textColor, false);
}

void Game::renderPauseMenu() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, this->width, 0, this->height);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLboolean lightingWasOn = glIsEnabled(GL_LIGHTING);
    if (lightingWasOn) {
        glDisable(GL_LIGHTING);
    }

    GLboolean depthTestWasOn = glIsEnabled(GL_DEPTH_TEST);
    if (depthTestWasOn) {
        glDisable(GL_DEPTH_TEST);
    }

    GLboolean previousDepthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &previousDepthMask);
    glDepthMask(GL_FALSE);

    GLboolean texture2DWasOn = glIsEnabled(GL_TEXTURE_2D);
    if (texture2DWasOn) {
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);

    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(this->width, 0);
        glVertex2f(this->width, this->height);
        glVertex2f(0, this->height);
    glEnd();

    float centerX = this->width / 2.0f;
    float centerY = this->height / 2.0f;

    const char* pauseText = "PAUSA";
    TTF_SetFontStyle(gameFont, TTF_STYLE_BOLD);
    drawTextGame(pauseText, centerX - (strlen(pauseText) * 10), centerY - 210, textColor, false);
    
    const char* resumeText = "Reanudar (R)";
    TTF_SetFontStyle(gameFont, TTF_STYLE_NORMAL);
    int resumeW, resumeH;
    TTF_SizeUTF8(gameFont, resumeText, &resumeW, &resumeH);
    resumeButtonRect = { (int)(centerX - resumeW / 2), (int)(centerY + 20), resumeW, resumeH + 5 };
    drawTextGame(resumeText, resumeButtonRect.x, this->height - (resumeButtonRect.y + resumeButtonRect.h), textColor, false);

    const char* wireframeText = "Alternar Wireframe (Y)"; //La W esta ocupada =(
    int wireW, wireH;
    TTF_SizeUTF8(gameFont, wireframeText, &wireW, &wireH);
    wireframeButtonRect = { (int)(centerX - wireW / 2), (int)(resumeButtonRect.y - wireH - 15), wireW, wireH + 5 };
    drawTextGame(wireframeText, wireframeButtonRect.x, this->height - (wireframeButtonRect.y + wireframeButtonRect.h), textColor, false);

    const char* textureText = "Alternar Texturas (T)";
    int texW, texH;
    TTF_SizeUTF8(gameFont, textureText, &texW, &texH);
    texturesButtonRect = { (int)(centerX - texW / 2), (int)(wireframeButtonRect.y - texH - 15), texW, texH + 5 };
    drawTextGame(textureText, texturesButtonRect.x, this->height - (texturesButtonRect.y + texturesButtonRect.h), textColor, false);

    const char* shadingText = "Alternar Facetado/Interpolado (F)";
    int shadeW, shadeH;
    TTF_SizeUTF8(gameFont, shadingText, &shadeW, &shadeH);
    shadingButtonRect = { (int)(centerX - shadeW / 2), (int)(texturesButtonRect.y - shadeH - 15), shadeW, shadeH + 5 };
    drawTextGame(shadingText, shadingButtonRect.x, this->height - (shadingButtonRect.y + shadingButtonRect.h), textColor, false);
    
    std::string gameSpeedText = "Velocidad del juego (<-, ->) ";
    gameSpeedText.append("x");
    char array[5];
    sprintf_s(array, "%.2f", gameSpeed);
    gameSpeedText.append(array);
    int speedW, speedH;
    TTF_SizeUTF8(gameFont, gameSpeedText.c_str(), &speedW, &speedH);
    gameSpeedButtonRect = { (int)(centerX - speedW / 2), (int)(shadingButtonRect.y - speedH - 15), speedW, speedH + 5 };
    drawTextGame(gameSpeedText.c_str(), gameSpeedButtonRect.x, this->height - (gameSpeedButtonRect.y + gameSpeedButtonRect.h), textColor, false);

    const char* mainMenuText = "Menu Principal (M)";
    int menuW, menuH;
    TTF_SizeUTF8(gameFont, mainMenuText, &menuW, &menuH);
    mainMenuButtonRect = { (int)(centerX - menuW / 2), (int)(gameSpeedButtonRect.y - menuH - 15), menuW, menuH + 5 };
    drawTextGame(mainMenuText, mainMenuButtonRect.x, this->height - (mainMenuButtonRect.y + mainMenuButtonRect.h), textColor, false);

    const char* exitText = "Salir del Juego (Q)";
    int exitW, exitH;
    TTF_SizeUTF8(gameFont, exitText, &exitW, &exitH);
    exitButtonRect = { (int)(centerX - exitW / 2), (int)(mainMenuButtonRect.y - exitH - 15), exitW, exitH + 5 };
    drawTextGame(exitText, exitButtonRect.x, this->height - (exitButtonRect.y + exitButtonRect.h), textColor, false);

    glDisable(GL_BLEND);

    if (texture2DWasOn) {
        glEnable(GL_TEXTURE_2D);
    }
    glDepthMask(previousDepthMask);
    if (depthTestWasOn) {
        glEnable(GL_DEPTH_TEST);
    }
    if (lightingWasOn) {
    glEnable(GL_LIGHTING);
}

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Game::render(int renderWidth, int renderHeight) {
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)renderWidth / (float)renderHeight, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera->Apply();

    if (this->rotate) {
        glRotatef(degrees, 0.0f, 1.0f, 0.0f);
    }

    setupLighting();

    drawAxis();
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    for (auto& gameObject : gameObjects) {
        gameObject->draw();
    }
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (gameState == PAUSED) {
        renderPauseMenu();
	}
}

Game::Game(SDL_Window* existingWindow, SDL_GLContext existingContext, TTF_Font* font,
           int gridSize, int winWidth, int winHeight, float camAngleX_param, float camAngleY_param, float radius_param, Camera* cam, Vector3 charPos)
    : score(0),
    step(0),
    width(winWidth),
    height(winHeight),
    rotate(false),
    degrees(0),
    camAngleX(camAngleX_param),
    camAngleY(camAngleY_param),
    radius(radius_param),
    lastMouseX(0),
    lastMouseY(0),
    isDragging(false),
    camera(cam),
    window(existingWindow),
    glctx(existingContext),
    grid(nullptr),
    characterPosition(charPos),
    worm(nullptr),
	gameObjects(std::vector <GameObject*>()),
    gameState(WAITING_FOR_INPUT),
    hudDisplayNumber(0),
    gameFont(font),
    textTextureCache(0)
{
    wireframe = false;
    grid = new CubeGrid(gridSize);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    lastTimestamp = 0;
    currentTimestamp = SDL_GetPerformanceCounter();
    gameSpeed = 1;
}

Game::~Game() {
    clearTextCache();
}

void Game::setHUDNumber(int number) {
    hudDisplayNumber = number;
}

int Game::getHUDNumber() const {
    return hudDisplayNumber;
}

void Game::processKey(const SDL_Event& event) {
    if (this->gameState == WAITING_FOR_INPUT || this->gameState == PROCESSING) {
        this->gameState = GameState::PROCESSING;
        Vector3 option;
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (this->willWormEatApple(this->worm->getOrientationForward())) {
                    this->growWorm();
                    this->gameState = GameState::ANIMATING;
                }
                else if (this->canWormMoveForward(this->worm->getOrientationForward())) {
                    this->worm->moveForward();
                    this->gameState = GameState::ANIMATING;
                    this->updateWormReferences();
                }
                break;
            case SDLK_UP:
                option = this->worm->calculateNewWormForwardOrientation(WormCommand::MOVE_UP);
                if (this->willWormEatApple(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_UP);
                    this->growWorm();
                    this->gameState = GameState::ANIMATING;
                }
                else if (this->canWormMoveForward(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_UP);
                    this->worm->moveForward();
                    this->updateWormReferences();
                    this->gameState = GameState::ANIMATING;
                }
                break;
            case SDLK_DOWN:
                option = this->worm->calculateNewWormForwardOrientation(WormCommand::MOVE_DOWN);
                if (this->willWormEatApple(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_DOWN);
                    this->growWorm();
                    this->gameState = GameState::ANIMATING;
                }
                else if (this->canWormMoveForward(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_DOWN);
                    this->worm->moveForward();
                    this->updateWormReferences();
                    this->gameState = GameState::ANIMATING;
                }
                break;
            case SDLK_RIGHT:
                option = this->worm->calculateNewWormForwardOrientation(WormCommand::MOVE_RIGHT);
                if (this->willWormEatApple(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_RIGHT);
                    this->growWorm();
                    this->gameState = GameState::ANIMATING;
                }
                else if (this->canWormMoveForward(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_RIGHT);
                    this->worm->moveForward();
                    this->updateWormReferences();
                    this->gameState = GameState::ANIMATING;
                }
                break;
            case SDLK_LEFT:
                option = this->worm->calculateNewWormForwardOrientation(WormCommand::MOVE_LEFT);
				std::cout << option.x << " " << option.y << " " << option.z << std::endl;
                if (this->willWormEatApple(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_LEFT);
                    this->growWorm();
                    this->gameState = GameState::ANIMATING;
                }
                else if (this->canWormMoveForward(option)) {
                    this->worm->updateNewWormOrientation(WormCommand::MOVE_LEFT);
                    this->worm->moveForward();
                    this->updateWormReferences();
                    this->gameState = GameState::ANIMATING;
                }
                break;
        }
        if (gameState != GameState::ANIMATING && gameState != GameState::FALLING && gameState != GameState::YOU_LOSE && gameState != GameState::YOU_WIN) {
            this->gameState = GameState::WAITING_FOR_INPUT;
        }
    }
}

GameLoopResult Game::loop() {
    bool gameRunning = true;
    SDL_Event event;
    bool ctrlPressed = false;

    lastTimestamp = SDL_GetPerformanceCounter();

    while (gameRunning) {
        currentTimestamp = SDL_GetPerformanceCounter();
        float deltaTime = (float)((currentTimestamp - lastTimestamp) / (double)SDL_GetPerformanceFrequency());
        lastTimestamp = currentTimestamp;

        while (SDL_PollEvent(&event)) {
            camera->HandleEvent(event);

            if (event.type == SDL_QUIT) {
                clearTextCache();
                return GameLoopResult::ExitApplication;
            }

            if (gameState == PAUSED) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_r) {
                        gameState = previousState;
                        clearTextCache();
                    } else if (event.key.keysym.sym == SDLK_m) {
                        clearTextCache();
                        return GameLoopResult::GoToMainMenu;
                    } else if (event.key.keysym.sym == SDLK_q) {
                        clearTextCache();
                        return GameLoopResult::ExitApplication;
                    } else if (event.key.keysym.sym == SDLK_t) {
                        toggleTextures();
                    } else if (event.key.keysym.sym == SDLK_y) {
                        toggleWireframe();
                    } else if (event.key.keysym.sym == SDLK_f) {
                        toggleShading();
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT) {
                        gameSpeed = gameSpeed + 0.25f;
                        if (gameSpeed > 2) {
                            gameSpeed = 2;
                        }
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        gameSpeed = gameSpeed - 0.25f;
                        if (gameSpeed < 0.25f) {
                            gameSpeed = 0.25f;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    int oglMouseY = mouseY;

                    if (mouseX >= resumeButtonRect.x && mouseX <= resumeButtonRect.x + resumeButtonRect.w &&
                        oglMouseY >= this->height - (resumeButtonRect.y + resumeButtonRect.h) && oglMouseY <= this->height - resumeButtonRect.y) {
                        gameState = WAITING_FOR_INPUT;
                        clearTextCache();
                    } else if (mouseX >= mainMenuButtonRect.x && mouseX <= mainMenuButtonRect.x + mainMenuButtonRect.w &&
                        oglMouseY >= (this->height - (mainMenuButtonRect.y + mainMenuButtonRect.h)) && oglMouseY <= (this->height - mainMenuButtonRect.y)) {
                        clearTextCache();
                        return GameLoopResult::GoToMainMenu;
                    } else if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                        oglMouseY >= (this->height - (exitButtonRect.y + exitButtonRect.h)) && oglMouseY <= (this->height - exitButtonRect.y)) {
                        clearTextCache();
                        return GameLoopResult::ExitApplication;
                    } else if (mouseX >= texturesButtonRect.x && mouseX <= texturesButtonRect.x + texturesButtonRect.w &&
                        oglMouseY >= (this->height - (texturesButtonRect.y + texturesButtonRect.h)) && oglMouseY <= (this->height - texturesButtonRect.y)) {
                        toggleTextures();
                    } else if (mouseX >= wireframeButtonRect.x && mouseX <= wireframeButtonRect.x + wireframeButtonRect.w &&
                        oglMouseY >= (this->height - (wireframeButtonRect.y + wireframeButtonRect.h)) && oglMouseY <= (this->height - wireframeButtonRect.y)) {
                        toggleWireframe();
                    } else if (mouseX >= shadingButtonRect.x && mouseX <= shadingButtonRect.x + shadingButtonRect.w &&
                        oglMouseY >= (this->height - (shadingButtonRect.y + shadingButtonRect.h)) && oglMouseY <= (this->height - shadingButtonRect.y)) {
                        toggleShading();
                    } else if (mouseX >= gameSpeedButtonRect.x && mouseX <= gameSpeedButtonRect.x + gameSpeedButtonRect.w &&
                        oglMouseY >= (this->height - (gameSpeedButtonRect.y + gameSpeedButtonRect.h)) && oglMouseY <= (this->height - gameSpeedButtonRect.y)) {
                        gameSpeed = gameSpeed + 0.25f;
                        if (gameSpeed > 2) {
                            gameSpeed = 0.25f;
                        }
                    }
                }
            } else {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        previousState = gameState;
                        gameState = PAUSED;
                    } else if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                        ctrlPressed = true;
                    } else {
                        if (gameState == WAITING_FOR_INPUT) {
                             processKey(event);
                        }
                    }
                } else if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                        ctrlPressed = false;
                        this->rotate = false;
                    }
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (ctrlPressed) { this->rotate = true; }
                } else if (event.type == SDL_MOUSEBUTTONUP) {
                    if (ctrlPressed) { this->rotate = false; }
                } else if (event.type == SDL_WINDOWEVENT) {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        this->width = event.window.data1;
                        this->height = event.window.data2;
                    }
                }
            }
        }

        if (gameState != PAUSED) {
            if (gameState == FALLING) {
                this->fallWorm();
                /*if (this->isWormSupported()) {
                    this->gameState = WAITING_FOR_INPUT;*/
                /*} else */if (this->worm && this->worm->tail && this->worm->tail->GetPosition().y < 0) {
                    this->gameState = YOU_LOSE;
                }
                else {
                    this->gameState = ANIMATING;
                }
            }
            else if (gameState == ANIMATING) {
                this->animateWorm(deltaTime * gameSpeed);
                if (worm && !worm->isAnimating()) {
                    if (!this->isWormSupported()) {
                        this->gameState = GameState::FALLING;
                    } else {
                        this->gameState = GameState::WAITING_FOR_INPUT;
                    }
                }
            }
            if (gameState == YOU_LOSE || gameState == YOU_WIN) {
                SDL_Delay(2000);
                clearTextCache();
                return GameLoopResult::GameEnded;
            }
        }
        
        render(this->width, this->height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16);
    }
    
    clearTextCache();
    return GameLoopResult::GameEnded;
}

void Game::destroy() {
    clearTextCache();
    delete worm; 
    worm = nullptr;
    delete grid;
    grid = nullptr;
    for (auto& go : gameObjects) {
        if (go) delete go;
    }
    gameObjects.clear();
}

bool Game::isWormSupported() {
    if (!worm || !grid) return false; 
    Vector3 headPos = worm->head->GetPosition();
    Vector3 headPosDown(headPos.x, headPos.y - 1, headPos.z);
    CubeGridElement* elementBelowHead = grid->at(headPosDown);
    if (elementBelowHead != nullptr && elementBelowHead->canSupportWormWeight()) {
        return true;
    }

    Vector3 tailPos = worm->tail->GetPosition();
    Vector3 tailPosDown(tailPos.x, tailPos.y - 1, tailPos.z);
    CubeGridElement* elementBelowTail = grid->at(tailPosDown);
    if (elementBelowTail != nullptr && elementBelowTail->canSupportWormWeight()) {
        return true;
    }
    
    for (const auto& part : worm->body) {
        if(!part) continue;
        Vector3 bodyPos = part->GetPosition();
        Vector3 bodyPosDown(bodyPos.x, bodyPos.y - 1, bodyPos.z);
        CubeGridElement* elementBelowBody = grid->at(bodyPosDown);
        if (elementBelowBody != nullptr && elementBelowBody->canSupportWormWeight()) {
            return true;
        }
    }
    return false;
}

void Game::updateWormFallInCubeGrid() {
    if (!worm || !grid) return;
	for (auto& part : worm->getParts()) {
        if(!part) continue;
		Vector3 pos = part->GetPosition();
        grid->setObject(pos, nullptr);
		Vector3 newPos(pos.x, pos.y - 1, pos.z);
        grid->setObject(newPos, part);
	}
}

void Game::fallWorm() {
    if (!worm || !grid) return;
    updateWormFallInCubeGrid();
    worm->fall(); 
    Vector3 tailPos = worm->tail->GetPosition();
    if (tailPos.y < 0) {
        gameState = YOU_LOSE;
    }
}

bool Game::canWormMoveForward(Vector3 orientation) {
    if (!worm || !grid) return false;
    Vector3 nextPos = worm->calculateWormPotentialNextPosition(orientation);
    CubeGridElement* slot = grid->at(nextPos);
    return slot != nullptr && !slot->isBlocked();
}

bool Game::willWormEatApple(Vector3 orientation) {
    if (!worm || !grid) return false;
    Vector3 nextPos = worm->calculateWormPotentialNextPosition(orientation);
    CubeGridElement* slot = grid->at(nextPos);
    return slot != nullptr && slot->hasApple();
}

void Game::updateWormReferences() {
    this->removeWormFromGameObjectsAndCubeGrid();
    this->addWormToGameObjectsAndCubeGrid();
}

void Game::removeGameObjectFromGameObjectsAndCubeGrid(GameObject* go, bool isWorm) {
    auto it = std::find(gameObjects.begin(), gameObjects.end(), go);
	std::cout << "Removing object: " << go->GetPosition().x << ", " << go->GetPosition().y << ", " << go->GetPosition().z << std::endl;
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
    Vector3 pos = go->GetPosition();
    this->grid->setObject(pos, nullptr);
    if (!isWorm) delete go;
}

void Game::eatApple() {
    GameObject* apple = this->grid->getObject(this->worm->getNextPosition());
    this->removeGameObjectFromGameObjectsAndCubeGrid(apple, false);
    this->score++;
}

void Game::addGameObjectToGameObjectsAndCubeGrid(GameObject* go) {
    this->gameObjects.push_back(go);
    //std::cout << "(" << go->GetPosition().x << "," << go->GetPosition().y << "," << go->GetPosition().z << ")";
    
    this->grid->setObject(go->GetPosition(), go);
}

void Game::growWorm() {
    this->eatApple();
	WormPart* newPart = this->worm->grow();
    this->updateWormReferences();
}

void Game::removeWormFromGameObjectsAndCubeGrid() {
	std::vector<GameObject*> wormParts = this->worm->getParts();
	for (auto& part : wormParts) {
		this->removeGameObjectFromGameObjectsAndCubeGrid(part, true);
	}
}

void Game::toggleWireframe() {
    wireframe = !wireframe;
    
    /*
    GLint* polygonMode = new int[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);
    if (polygonMode[0] == GL_LINE || polygonMode[1] == GL_LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    delete[] polygonMode;*/
}

void Game::toggleTextures() {
    GLboolean a = GLboolean();
    glGetBooleanv(GL_TEXTURE_2D, &a);
    //std::cout << a;
    if (a == true) {
        glDisable(GL_TEXTURE_2D);
    }
    else {
        glEnable(GL_TEXTURE_2D);
    }
}

void Game::toggleShading() {
    GLint a = GLint();
    glGetIntegerv(GL_SHADE_MODEL,&a);
    //std::cout << a;
    if (a == GL_FLAT) {
        glShadeModel(GL_SMOOTH);
    }
    else {
        glShadeModel(GL_FLAT);
    }
}

void Game::addWormToGameObjectsAndCubeGrid() {
    std::vector<GameObject*> wormParts = this->worm->getParts();
    for (auto& part : wormParts) {
        WormPart* wpart = static_cast<WormPart*>(part);
        this->gameObjects.push_back(part);
        this->grid->setObject(wpart->getAnimatorEndPosition(), part);
    }
}

void Game::animateWorm(float deltaTime) {
    this->worm->updateAnimation(deltaTime);
}