#pragma once
#include "WormPart.h"
#include "Colors.h"
#include "Utils.h"
#include <iostream>

WormPart::WormPart(const Vector3 pos, WormPartType type)
    : GameObject(pos), type(type) {
	this->anim = new Animator(this);
	this->anim->SetPosition(pos);
}


void WormPart::draw() {
	Vector3 position = this->GetPosition();
	drawCube(position.x, position.y, position.z, 1.0f, Colors::GREEN);
	//std::cout << (int)type << "drawn at (" << position.x << "," << position.y << "," << position.z << ") \n";
}

void WormPart::SetType(WormPartType type) {
	this->type = type;
}

void WormPart::animatorMove(Vector3 newPosition) {
	anim->AnimateLinear(this->GetPosition(),newPosition,0.3f);
}

void WormPart::updateAnimator(float deltaTime) {
	this->anim->Update(deltaTime);
}

void WormPart::update(float deltaTime) {
	this->anim->Update(deltaTime);
}

bool WormPart::isAnimating() {
	return this->anim->IsAnimating();
}

Vector3 WormPart::getAnimatorEndPosition() {
	if (anim != nullptr) {
	
	
		return this->anim->GetEndPosition();
	}
}
