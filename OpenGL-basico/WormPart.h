#pragma once
#include "GameObject.h"
#include "Vector3.h"

enum class WormPartType { Head, Body, Tail };

class WormPart : public GameObject {
public:
    WormPartType type;

    WormPart(const Vector3 pos, WormPartType type);
	void SetType(WormPartType type);

    void draw() override;
};