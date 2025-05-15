#include "CubeGridElement.h"

CubeGridElement::CubeGridElement(Vector3 position)
    : position(position), object(nullptr) {
}


void CubeGridElement::setObject(GameObject * obj) {
	object = obj;
}

GameObject* CubeGridElement::getObject() {
	return object;
}

bool CubeGridElement::isBlockedByTerrain() {
	return dynamic_cast<Terrain*>(object) != nullptr;
}

bool CubeGridElement::canSupportWormWeight() {
	return dynamic_cast<Terrain*>(object) != nullptr || dynamic_cast<Apple*>(object) != nullptr;
} 

bool CubeGridElement::hasApple() {
	return dynamic_cast<Apple*>(object) != nullptr;
}