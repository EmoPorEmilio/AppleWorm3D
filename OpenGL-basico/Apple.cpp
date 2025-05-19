#pragma once
#include "Apple.h"
#include "Vector3.h"
#include "Colors.h"
#include "Utils.h"

Apple::Apple(Vector3 coordinates) : GameObject(coordinates) {
	logicPos = coordinates;
	anim = new Animator(this);
	anim->SetPosition(coordinates);
	up = true;
}

void Apple::draw() {
	Vector3 position = this->GetPosition();
	drawCube(position.x, position.y, position.z, 1.0f, Colors::RED);
}

void Apple::update(float deltaTime) {
	if (!this->anim->IsAnimating()) {
		if (up) {
			anim->AnimateLinear(logicPos,logicPos + Vector3(0,0.1,0),5 );
		}
		else {
			anim->AnimateLinear(logicPos + Vector3(0, 0.1, 0),logicPos, 5);
		}
		up = !up;
	}
	anim->Update(deltaTime);
}