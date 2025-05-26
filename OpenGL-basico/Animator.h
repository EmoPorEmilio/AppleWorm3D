#pragma once

#include "Vector3.h"
#include "GameObject.h"

#define ANIM_LINEAR 0
#define ANIM_QUAD 1



class Animator {
private:
	GameObject* go;
	float timer;
	float totalTimer;
	Vector3 initPos;
	Vector3 endPos;
	Vector3 distance;
	float speed;
	float acceleration;
	void MovePosition();
public:
	Animator(GameObject* go);
	Animator();
	~Animator();
	void AddGameObject(GameObject* go);
	bool IsAnimating();
	void AnimateLinear(Vector3 initPos, Vector3 endPos, float time);
	void AnimateQuad(Vector3 initPos, Vector3 endPos, Vector3 accel);
	//void AnimateLinear(Vector3 initPos, Vector3 endPos, Vector3 speed);
	//void AnimateQuad(Vector3 initPos, Vector3 endPos, Vector3 speed, Vector3 accel);
	void Update(float deltaTime);
	void SetPosition(Vector3 endPos);
	Vector3 GetObjectPosition();
	Vector3 GetEndPosition();
};