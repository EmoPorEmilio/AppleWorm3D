#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "Camera.h"
#include "Vector3.h"

using namespace std;

// Función para dibujar un cubo sin usar GLUT
void drawCube() {
	glBegin(GL_QUADS);
	// Cara frontal (z = 0.5)
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	
	// Cara trasera (z = -0.5)
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	
	// Cara superior (y = 0.5)
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	
	// Cara inferior (y = -0.5)
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	
	// Cara derecha (x = 0.5)
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	
	// Cara izquierda (x = -0.5)
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();
}

int main(int argc, char *argv[]) {
	//INICIALIZACION
	if (SDL_Init(SDL_INIT_VIDEO)<0) {
		cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("ICG-UdelaR",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext context = SDL_GL_CreateContext(win);

	glMatrixMode(GL_PROJECTION);

	float color = 0;
	glClearColor(color, color, color, 1);

	gluPerspective(45, 640 / 480.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	bool fin = false;
	bool rotate = false;
	bool ctrlPressed = false;

	SDL_Event evento;

	Vector3 characterPosition(0.0f, 0.0f, 0.0f);
	
	Camera camera;
	camera.Initialize(0.0f, 0.0f, 5.0f);
	camera.SetCharacterReference(&characterPosition);

	float degrees = 0;

	SDL_SetRelativeMouseMode(SDL_TRUE);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		
		// Actualizar la cámara (especialmente importante para modo de seguimiento)
		camera.Update();
		
		// Aplicar la cámara
		camera.Apply();

		if (rotate) {
			degrees = degrees + 0.1f;
		}
		glRotatef(degrees, 0.0, 1.0, 0.0);

		// Dibujar un triángulo (como ejemplo)
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(1., -1., 0.);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1., -1., 0.);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0., 1., 0.);
		glEnd();
		
		// Dibujar un cubo para representar al personaje
		glPushMatrix();
		glTranslatef(characterPosition.x, characterPosition.y, characterPosition.z);
		glColor3f(0.0, 1.0, 0.0);
		drawCube();
		glPopMatrix();
		//FIN DIBUJAR OBJETOS

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)){
			// Procesar eventos para la cámara
			camera.HandleEvent(evento);
			
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				// Solo activar rotación si CTRL está presionado
				if (ctrlPressed) {
					rotate = true;
					cout << "ROT\n";
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (ctrlPressed) {
					rotate = false;
				}
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYDOWN:
				// Comprobar CTRL
				if (evento.key.keysym.sym == SDLK_LCTRL || evento.key.keysym.sym == SDLK_RCTRL) {
					ctrlPressed = true;
				}
				// Mover al personaje con teclas de flecha
				else if (evento.key.keysym.sym == SDLK_UP) {
					characterPosition.z -= 0.1f;
				}
				else if (evento.key.keysym.sym == SDLK_DOWN) {
					characterPosition.z += 0.1f;
				}
				else if (evento.key.keysym.sym == SDLK_LEFT) {
					characterPosition.x -= 0.1f;
				}
				else if (evento.key.keysym.sym == SDLK_RIGHT) {
					characterPosition.x += 0.1f;
				}
				break;
			case SDL_KEYUP:
				if (evento.key.keysym.sym == SDLK_LCTRL || evento.key.keysym.sym == SDLK_RCTRL) {
					ctrlPressed = false;
					// Si soltamos CTRL, también dejamos de rotar
					rotate = false;
				}
				else if (evento.key.keysym.sym == SDLK_ESCAPE) {
					fin = true;
				}
				break;
			}
		}
		//FIN MANEJO DE EVENTOS
		SDL_GL_SwapWindow(win);
	} while (!fin);
	//FIN LOOP PRINCIPAL
	// LIMPIEZA
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
