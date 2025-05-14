#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Apple.h"

class CubeGridElement {
public:
	const Vector3 position;
	CubeGridElement(Vector3 position);
private:
    GameObject* object;

	/*
	void setObject(GameObject* obj);

	GameObject* getObject();

	bool isBlockedByTerrain();

	bool canSupportWormWeight();*/
};