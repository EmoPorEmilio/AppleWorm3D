#pragma once
#include "WormPart.h"
#include "Colors.h"
#include "Utils.h"

WormPart::WormPart(const Vector3 pos, WormPartType type)
    : GameObject(pos), type(type) {
}


void WormPart::draw() {
	drawCube(position.x, position.y, position.z, 1.0f, Colors::GREEN);
}