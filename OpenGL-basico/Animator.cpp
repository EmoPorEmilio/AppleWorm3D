#include "Animator.h"
#include <iostream>
#include <cmath>

using namespace std;

Animator::Animator() {
	this->timer = 0;
	this->totalTimer = 0;
	this->go = NULL;
	this->distance = Vector3();
	this->endPos = Vector3();
	this->initPos = Vector3();
	//Currently unimplemented
	this->acceleration = 0;
	this->speed = 0;
}

Animator::Animator(GameObject* go) {
	this->Animator::Animator();
	this->go = go;
}

Animator::~Animator() {
	delete go;
}

void Animator::AddGameObject(GameObject* go) {
	this->go = go;
}

bool Animator::IsAnimating() {
	if (this->timer > 0) {
		return true;
	}
	this->go->SetPosition(endPos);
	return false;
}

void Animator::AnimateLinear(Vector3 initPos, Vector3 endPos, float time) {
	this->distance = endPos - initPos;
	this->totalTimer = time;
	this->initPos = initPos;
	this->endPos = endPos;
	this->timer = time;
}

void Animator::Update(float deltaTime) {
	if (this->IsAnimating()) {
		//cout << timer << endl;
		this->timer = this->timer - deltaTime;
		MovePosition();
	}
	else {
		if (go != NULL) {
			go->SetPosition(endPos);
			
		}
	}
}

void Animator::MovePosition() {
	if (timer > 0) {
		if (acceleration == 0 && go != NULL) {
			//cout << "linear anim\n";
			this->go->SetPosition(initPos + distance * ((totalTimer - timer) / totalTimer));
		}
		else if (acceleration > 0 && go != NULL) {
			Vector3 newPos = initPos + distance.Normalized() * ((1 / (float)2) * acceleration * (totalTimer - timer) * (totalTimer - timer));
			this->go->SetPosition(newPos);
		}
	}
}

void Animator::AnimateQuad(Vector3 initPos, Vector3 endPos, Vector3 accel) {
	distance = endPos - initPos;
	this->endPos = endPos;
	this->initPos = initPos;
	float a = accel.Length();
	this->acceleration = a;
	float xf = distance.Length();
	this->timer = (sqrt((2 * xf) / (a)));
	this->totalTimer = timer;
	this->speed = 0;
}

void Animator::SetPosition(Vector3 endPos) {
	this->endPos = endPos;
}

Vector3 Animator::GetObjectPosition() {
	return this->go->GetPosition();
}