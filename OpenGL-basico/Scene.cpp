#include "Scene.h"
#include <iostream>
#include "Terrain.h"
#include "Worm.h"
#include "GameObject.h"

Scene::Scene() {
	gameObjects = std::vector<GameObject*>();
	worm = NULL;
	//snake = vector<Animator*>();
}

Scene::~Scene() {
	int size = gameObjects.size();
	for (int i = 0; i < size; i++) {
		delete gameObjects.at(i);
	}
	//size = snake.size();
	/*for (int i = 0; i < size; i++) {
		delete snake.at(i);
	}*/
}

void Scene::Draw() {
	int size = gameObjects.size();
	for (int i = 0; i < size; i++) {
		gameObjects.at(i)->draw();
	}
	//worm->draw();
}

void Scene::Update(float deltaTime) {
	size_t size = animators.size();
	for (size_t i = 0; i < size; i++) {
		animators.at(i)->Update(deltaTime);
	}
	if (worm != NULL) {
		//worm->Update(deltaTime);
	}
	Draw();
}

void Scene::AddGameObject(GameObject* gameObject) {
	gameObjects.push_back(gameObject);
}

void Scene::AddFloor(std::vector<Vector3> positions) {
	for (int i = 0; i < positions.size(); i++) {
		Terrain* terrain = new Terrain(positions[i]);
		this->AddGameObject(terrain);
	}
}

void Scene::AddSnake(std::vector<Vector3> position) {
	//this->worm = new Worm(position.at(0), position.at(1), position.at(2), position.at(3));
	/*int j = position.size();

	for (int i = 0; i < j; i++) {
		this->AddSnake(position[i]);
	}*/
}

//void Scene::AddSnake(Vector3 position) {
//	//Make the gameObject
//	Vector3 wormColor = Vector3(0.0f, 0.60f, 0);
//	GameObject* go = new GameObject(GAMEOBJECT_CUBE, wormColor);
//	go->SetPosition(position);
//	this->AddGameObject(go);
//	//Add an animator
//	Animator* anim = new Animator();
//	anim->AddGameobject(go);
//	anim->SetPosition(position);
//	snake.push_back(anim);
//}
//
void Scene::MoveSnake(int i, Vector3 position, int type) {
	//std::vector<Animator*> snakeAnim = this->worm->GetAnimators();
	//snakeAnim.at(i)->AnimateLinear(snakeAnim.at(i)->GetObjectPosition(), position, 0.7f);
}
//
//
//
bool Scene::IsSnakeMoving() {
	/*
	std::vector<Animator*> snake = this->worm->GetAnimators();
	bool res = false;
	size_t size = snake.size();
	for (int i = 0; i < size; i++) {
		res = res || snake.at(i)->IsAnimating();
		if (res) {
			return res;
		}
	}
	return res;*/
	return false;
}
//
//void Scene::AddFloor(Vector3 position) {
//	Vector3 dirtColor = Vector3(0.439f, 0.227f, 0);
//	GameObject* go = new GameObject(GAMEOBJECT_CUBE, dirtColor);
//	go->SetPosition(position);
//	this->AddGameObject(go);
//}
//
//
//}
//
//void Scene::AddApple(Vector3 position) {
//	Vector3 dirtColor = Vector3(1, 0.3f, 0.3f);
//	GameObject* go = new GameObject(GAMEOBJECT_CUBE, dirtColor);
//	go->SetPosition(position);
//	this->AddGameObject(go);
//}

//void Scene::AddAnimObj(GameObject* go) {
//	this->AddGameObject(go);
//	Animator* anim = new Animator();
//	anim->AddGameobject(go);
//	anim->SetPosition(go->GetPosition());
//	other.push_back(anim);
//}
//
//void Scene::AnimObj(int i, Vector3 position, int type) {
//	switch (type) {
//	case (ANIM_LINEAR):
//		other.at(i)->AnimateLinear(other.at(i)->GetObjectPosition(), position, 0.7f);
//		break;
//	case (ANIM_QUAD):
//		other.at(i)->AnimateQuad(other.at(i)->GetObjectPosition(), position, Vector3(0, -9.8, 0));
//		break;
//	}
//}