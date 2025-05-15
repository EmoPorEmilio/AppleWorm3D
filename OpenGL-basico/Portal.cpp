#pragma once
#include "Portal.h"
#include "Colors.h"
#include "Utils.h"

Portal::Portal(Vector3 coordinates) : GameObject(coordinates) {
}

void Portal::draw() {
	drawCube(position.x, position.y, position.z, 1.0f, Colors::BLUE);
}