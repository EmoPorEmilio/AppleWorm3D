#pragma once
#include "Terrain.h"
#include "Vector3.h"
#include "Colors.h"
#include "Utils.h"

Terrain::Terrain(Vector3 coordinates) : GameObject(coordinates) {
}

void Terrain::draw() {
	drawCube(position.x, position.y, position.z, 1.0f, Colors::BROWN);
}