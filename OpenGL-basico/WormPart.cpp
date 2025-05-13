#pragma once
#include "WormPart.h"
#include "Coordinates3D.h"
#include "Colors.h"

WormPart::WormPart(const Coordinates3D& pos, WormPartType type)
    : GameObject(pos), type(type) {
}


void WormPart::draw() {
    
}