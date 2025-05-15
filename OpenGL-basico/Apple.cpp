#pragma once
#include "Apple.h"
#include "Vector3.h"
#include "Colors.h"
#include "Utils.h"

Apple::Apple(Vector3 coordinates) : GameObject(coordinates) {
}

void Apple::draw() {
	Vector3 position = this->GetPosition();
	drawCube(position.x, position.y, position.z, 1.0f, Colors::RED);
}