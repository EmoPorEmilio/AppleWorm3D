#pragma once
#include "GameObject.h"

enum class WormPartType { Head, Body, Tail };

class WormPart : public GameObject {
public:
    WormPartType type;

    WormPart(const Coordinates3D& pos, WormPartType type);

    void draw() override;
};