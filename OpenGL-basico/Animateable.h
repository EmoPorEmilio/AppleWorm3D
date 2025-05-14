#pragma once

#include <vector>
#include "Vector3.h"

class Animateable {
public:
	void virtual SetPosition(Vector3 position) = 0;
	Vector3 virtual GetPosition() = 0;
};