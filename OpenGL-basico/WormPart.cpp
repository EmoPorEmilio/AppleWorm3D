#pragma once
#include "WormPart.h"
#include "Colors.h"
#include "Utils.h"
#include <iostream>

WormPart::WormPart(const Vector3 pos, WormPartType type)
    : GameObject(pos), type(type) {
}


void WormPart::draw() {
	Vector3 position = this->GetPosition();
	drawCube(position.x, position.y, position.z, 1.0f, Colors::GREEN);
}

void WormPart::SetType(WormPartType type) {
	this->type = type;
}