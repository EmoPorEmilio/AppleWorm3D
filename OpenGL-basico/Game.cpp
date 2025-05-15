#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "Game.h"
#include "Colors.h"
#include "WormPart.h"
#include "Worm.h"
#include "Terrain.h"
#include "Apple.h"
#include "Portal.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "CubeGrid.h"
#include "tinyxml/tinyxml.h"
#include "Utils.h"

void Game::loadGameObjectsFromXML(const char* filename) {
    TiXmlDocument doc(filename);

    if (doc.LoadFile()) {
        float size = 0;
        TiXmlElement* element = doc.FirstChildElement();
        element->QueryFloatAttribute("size", &size);
        this->grid = new CubeGrid(size);
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
                    auto value = current_element->Value();
                    if (strcmp(value, "Head") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        head = Vector3(x, y, z);
                    }
                    else if (strcmp(value, "Body") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        body = Vector3(x, y, z);
                    }
                    else if (strcmp(value, "Tail") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
                        tail = Vector3(x, y, z);
                    }
					else if (strcmp(value, "OrientationForward") == 0) {
                        current_element->QueryFloatAttribute("x", &x);
                        current_element->QueryFloatAttribute("y", &y);
                        current_element->QueryFloatAttribute("z", &z);
						orientationForward = Vector3(x, y, z);
					}
                    else if (strcmp(value, "OrientationUp") == 0) {
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

				Worm* worm = new Worm(headPart, bodyPart, tailPart, orientationForward, orientationUp);
                this->grid->setObject(head, headPart);
                this->grid->setObject(body, bodyPart);
                this->grid->setObject(tail, tailPart);
                this->gameObjects.push_back(headPart);
                this->gameObjects.push_back(bodyPart);
                this->gameObjects.push_back(tailPart);
                this->worm = worm;
            }
            else if ((strcmp(value, "Terrain") == 0)) {
                Terrain* terrain = new Terrain(Vector3(x, y, z));
                this->gameObjects.push_back(terrain);
				this->grid->setObject(Vector3(x, y, z), terrain);
            }
            else if ((strcmp(value, "Apple") == 0)) {
                Apple* apple = new Apple(Vector3(x, y, z));
                this->gameObjects.push_back(apple);
                this->grid->setObject(Vector3(x, y, z), apple);
            }
            else if ((strcmp(value, "Portal") == 0)) {
                Portal* portal = new Portal(Vector3(x, y, z));
                this->gameObjects.push_back(portal);
                this->grid->setObject(Vector3(x, y, z), portal);
            }

            element = element->NextSiblingElement();
        }
    }
    else {
        std::cerr << "Failed to load XML file: " << doc.ErrorDesc() << std::endl;
    }
}

void Game::setupLighting() {
    //GLfloat light_pos[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat light_amb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    //GLfloat light_dif[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    //GLfloat light_spe[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, light_spe);
}

void Game::drawAxis(void)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(-50, 0, 0);
    glVertex3f(50, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, -50, 0);
    glVertex3f(0, 50, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -50);
    glVertex3f(0, 0, 50);
    glEnd();
    glEnable(GL_LIGHTING);
}

void Game::render(int width, int height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 100.0);

    // Modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    

    // Camera orbit
    float camX = radius * sinf(camAngleY * 3.14159f / 180.0f) *
        cosf(camAngleX * 3.14159f / 180.0f);
    float camY = radius * sinf(camAngleX * 3.14159f / 180.0f);
    float camZ = radius * cosf(camAngleY * 3.14159f / 180.0f) *
        cosf(camAngleX * 3.14159f / 180.0f);

    gluLookAt(camX, camY, camZ, 10, 10, 10, 0, 1, 0);

    setupLighting();

    drawAxis();
    
	for (auto& gameObject : gameObjects) {
		gameObject->draw();
	}

    /*
    drawCube(0, 1, 2, 1.0, Colors::RED);
    //first row
    drawCube(0, 0, 0, 1.0, Colors::BROWN);
    drawCube(1, 0, 0, 1.0, Colors::BROWN);
    drawCube(2, 0, 0, 1.0, Colors::BROWN);
    drawCube(3, 0, 0, 1.0, Colors::BROWN);
    drawCube(4, 0, 0, 1.0, Colors::BROWN);
    drawCube(5, 0, 0, 1.0, Colors::BROWN);
    //second row
    drawCube(0, 0, 1, 1.0, Colors::BROWN);
    drawCube(1, 0, 1, 1.0, Colors::BROWN);
    drawCube(2, 0, 1, 1.0, Colors::BROWN);
    drawCube(3, 0, 1, 1.0, Colors::BROWN);
    drawCube(4, 0, 1, 1.0, Colors::BROWN);
    drawCube(5, 0, 1, 1.0, Colors::BROWN);
    //third row
    drawCube(0, 0, 2, 1.0, Colors::BROWN);
    drawCube(1, 0, 2, 1.0, Colors::BROWN);
    drawCube(2, 0, 2, 1.0, Colors::BROWN);
    drawCube(3, 0, 2, 1.0, Colors::BROWN);
    drawCube(4, 0, 2, 1.0, Colors::BROWN);
    drawCube(5, 0, 2, 1.0, Colors::BROWN);

    //worm and apple
    drawCube(2, 1, 0, 1.0, Colors::GREEN);
    drawCube(1, 1, 0, 1.0, Colors::GREEN);
    drawCube(0, 1, 0, 1.0, Colors::GREEN);
    drawCube(5, 1, 0, 1.0, Colors::BLUE);*/
}

Game::Game(int gridSize, int width, int height, float camAngleX, float camAngleY, float radius, Camera* camera, Vector3 characterPosition)
    : score(0),
    step(0),
    width(width),
    height(height),
    camAngleX(camAngleX),
    camAngleY(camAngleY),
    radius(radius),
    lastMouseX(0),
    lastMouseY(0),
    isDragging(false),
    camera(camera),
    window(nullptr),
    glctx(nullptr),
    grid(nullptr),
    characterPosition(characterPosition),
    worm(nullptr),
	gameObjects(std::vector <GameObject*>()),
	gameState(WAITING_FOR_INPUT)
{
    grid = new CubeGrid(gridSize);
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "SDL2 + OpenGL 2.1 Cube",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    glctx = SDL_GL_CreateContext(window);

    glEnable(GL_DEPTH_TEST);
}

void Game::processKey(const SDL_Event& event) {
    if (this->gameState == GameState::WAITING_FOR_INPUT) {
        this->gameState = GameState::PROCESSING;
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (this->willWormEatApple()) {
                    this->growWorm();
                }
                else if (this->canWormMoveForward()) {
                    this->worm->moveForward();
                }
                break;
            case SDLK_UP:
                this->worm->calculateNewWormOrientation(WormCommand::MOVE_UP);
                if (this->willWormEatApple()) {
                    this->growWorm();
                }
                else if (this->canWormMoveForward()) {
                    this->worm->moveForward();
                }
                break;
            case SDLK_DOWN:
                this->worm->calculateNewWormOrientation(WormCommand::MOVE_DOWN);
                if (this->willWormEatApple()) {
                    this->growWorm();
                }
                else if (this->canWormMoveForward()) {
                    this->worm->moveForward();
                }
                break;
            case SDLK_RIGHT:
                this->worm->calculateNewWormOrientation(WormCommand::MOVE_RIGHT);
                if (this->willWormEatApple()) {
                    this->growWorm();
                }
                else if (this->canWormMoveForward()) {
                    this->worm->moveForward();
                }
                break;
            case SDLK_LEFT:
                this->worm->calculateNewWormOrientation(WormCommand::MOVE_LEFT);
                if (this->willWormEatApple()) {
                    this->growWorm();
                }
                else if (this->canWormMoveForward()) {
                    this->worm->moveForward();
                }
                break;
        }
        this->gameState = GameState::WAITING_FOR_INPUT;
		if (!this->isWormSupported()) {
            this->gameState = GameState::FALLING;
		}
    }
}

bool Game::isWormSupported() {
    //naive implementation
    //por ahora chequeo todo cada vez, pero en mundo ideal en cada movimiento solo se deberían
    //recalcular las posiciones que cambian, no todas
    Vector3 headPosition = this->worm->head->GetPosition();
    Vector3 headPositionDown = Vector3(headPosition.x, headPosition.y - 1, headPosition.z);
    Vector3 tailPosition = this->worm->tail->GetPosition();
    Vector3 tailPositionDown = Vector3(tailPosition.x, tailPosition.y - 1, tailPosition.z);
    if (this->grid->at(headPositionDown)->canSupportWormWeight()) {
        return true;
    }
    if (this->grid->at(tailPositionDown)->canSupportWormWeight()) {
        return true;
    }
    for (auto& part : this->worm->body) {
        Vector3 bodyPosition = part->GetPosition();
        Vector3 bodyPositionDown = Vector3(bodyPosition.x, bodyPosition.y - 1, bodyPosition.z);
        if (this->grid->at(bodyPosition)->canSupportWormWeight()) {
            return true;
        }
    }
    return false;
};

void Game::updateWormFallInCubeGrid() {
	for (auto& part : this->worm->getParts()) {
		Vector3 pos = part->GetPosition();
		Vector3 newPos = Vector3(pos.x, pos.y - 1, pos.z);
		this->grid->setObject(pos, nullptr);
        this->grid->setObject(newPos, part);
	}
}

void Game::fallWorm() {
    if (!this->isWormSupported()) {
        this->updateWormFallInCubeGrid();
        this->worm->fall(); 
        if (this->worm->tail->GetPosition().y < 5) {
            this->gameState = GameState::YOU_LOSE;
        }
    }
}

bool Game::canWormMoveForward() {
    Vector3 nextPos = this->worm->getNextPosition();
    CubeGridElement* slot = this->grid->at(nextPos);
    bool isBlocked = slot->isBlockedByTerrain();
    return !isBlocked;
}

bool Game::willWormEatApple() {
    return this->grid->at(this->worm->getNextPosition())->hasApple();
}

void Game::removeGameObjectFromGameObjectsAndCubeGrid(GameObject* go, bool isWorm) {
    auto it = std::find(gameObjects.begin(), gameObjects.end(), go);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
    Vector3 pos = go->GetPosition();
    this->grid->setObject(pos, nullptr);
    if (!isWorm) { delete go; };
}

void Game::eatApple() {
    GameObject* apple = this->grid->getObject(this->worm->getNextPosition());
    this->removeGameObjectFromGameObjectsAndCubeGrid(apple, false);
    this->score++;
}

void Game::addGameObjectToGameObjectsAndCubeGrid(GameObject* go) {
    this->gameObjects.push_back(go);
    this->grid->setObject(go->GetPosition(), go);
}

void Game::growWorm() {
    this->eatApple();
	WormPart* newPart = this->worm->grow();
    this->addGameObjectToGameObjectsAndCubeGrid(newPart);
}

void Game::removeWormFromGameObjectsAndCubeGrid() {
	std::vector<GameObject*> wormParts = this->worm->getParts();
	for (auto& part : wormParts) {
		this->removeGameObjectFromGameObjectsAndCubeGrid(part, true);
	}
}

void Game::addWormToGameObjectsAndCubeGrid() {
    std::vector<GameObject*> wormParts = this->worm->getParts();
    for (auto& part : wormParts) {
        gameObjects.push_back(part);
    }
}

void Game::loop() {
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = true;
                    lastMouseX = event.button.x;
                    lastMouseY = event.button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (isDragging) {
                    int dx = event.motion.x - lastMouseX;
                    int dy = event.motion.y - lastMouseY;
                    camAngleY += dx;
                    camAngleX += dy;
                    if (camAngleX > 89) camAngleX = 89;
                    if (camAngleX < -89) camAngleX = -89;
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                }
                break;
            case SDL_MOUSEWHEEL:
                // Zoom in/out
                radius -= event.wheel.y * 0.5f; // y > 0 is scroll up
                if (radius < 2.0f) radius = 2.0f;
                if (radius > 20.0f) radius = 20.0f;
                break;
            case SDL_KEYUP:
                this->processKey(event);
                break;
			}
		}
		if (this->gameState == GameState::FALLING) {
			this->fallWorm();
			if (this->isWormSupported()) {
				this->gameState = GameState::WAITING_FOR_INPUT;
			}
		}
        render(width, height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }
}

/*intentando integrar cámara nueva void 
Game::loop() {
    bool running = true;
    SDL_Event event;
    bool rotate = false;
    bool ctrlPressed = false;
    float degrees = 0;

    //SDL_SetRelativeMouseMode(SDL_TRUE);
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Procesar eventos para la cámara
            camera->HandleEvent(event);

            switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                // Solo activar rotación si CTRL está presionado
                if (ctrlPressed) {
                    rotate = true;
                    std::cout << "ROT\n";
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (ctrlPressed) {
                    rotate = false;
                }
                break;
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                // Comprobar CTRL
                if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                    ctrlPressed = true;
                }
                // Mover al personaje con teclas de flecha
                else if (event.key.keysym.sym == SDLK_UP) {
                    characterPosition.z -= 0.1f;
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    characterPosition.z += 0.1f;
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    characterPosition.x -= 0.1f;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    characterPosition.x += 0.1f;
                }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) {
                    ctrlPressed = false;
                    // Si soltamos CTRL, también dejamos de rotar
                    rotate = false;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                break;
            }
        }

        render(width, height);
        SDL_GL_SwapWindow(window);
        SDL_Delay(16); // ~60 FPS
    }
}*/

void Game::destroy() {
    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
}