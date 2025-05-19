#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Apple.h"
#include "Portal.h"
#include "WormPart.h"

class CubeGridElement {
private:
	GameObject* object;
	const Vector3 position;
public:
	CubeGridElement(Vector3 position);
	void setObject(GameObject* obj);
	GameObject* getObject();
	bool isBlocked();
	bool canSupportWormWeight();
	bool hasApple();
	bool hasPortal();
};