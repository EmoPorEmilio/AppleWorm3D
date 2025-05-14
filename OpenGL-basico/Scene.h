#pragma once
#include <vector>
#include "GameObject.h"
#include "Worm.h"
#include "Animator.h"
#include "Vector3.h"
#include "Terrain.h"

class Scene {
private:
	std::vector<GameObject*> gameObjects;
	std::vector<Animator*> animators;
	Worm* worm;
public:
	Scene();
	~Scene();

	void Draw();
	void Update(float deltaTime);

	void AddGameObject(GameObject* gameObject);
	void AddFloor(std::vector<Vector3> positions);
	void AddSnake(std::vector<Vector3> position);
	//void AddAnimObj(GameObject* go);

	//Snake Related
	//Single Segment
	//void AddSnake(Vector3 position);
	//Array of segments
	//

	//Mostly test functions
	void MoveSnake(int i, Vector3 position, int type);
	bool IsSnakeMoving();

	//Floor Related
	//void AddFloor(Vector3 position);
	////Array of ground


	////Apple Related, Not convinced Yet
	//void AddApple(Vector3 position);
	///*void AddAnimObj(GameObject *go);
	//void AnimObj(int i, Vector3 position, int type);*/
	////Array of apples
	//void AddApple(std::vector<Vector3> positions);
};
