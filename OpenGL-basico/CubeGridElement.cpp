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

bool CubeGridElement::isBlocked() {
	return (dynamic_cast<Terrain*>(object) != nullptr || dynamic_cast<WormPart*>(object) != nullptr);
}

bool CubeGridElement::canSupportWormWeight() {
	return dynamic_cast<Terrain*>(object) != nullptr || dynamic_cast<Apple*>(object) != nullptr;
} 

bool CubeGridElement::hasApple() {
	return dynamic_cast<Apple*>(object) != nullptr;
}

bool CubeGridElement::hasPortal() {
	return dynamic_cast<Portal*>(object) != nullptr;
}

bool CubeGridElement::hasLava() {
	return dynamic_cast<Lava*>(object) != nullptr;
}