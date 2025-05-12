#pragma once
#include <vector>
#include "WormPart.h"
#include "Coordinates3D.h"

class Worm {
public:
    WormPart head;
    WormPart tail;
    std::vector<WormPart> body;
    Coordinates3D orientation; // Now just a single direction

    Worm(const Coordinates3D& headPos,
        const Coordinates3D& bodyPos,
        const Coordinates3D& tailPos,
        const Coordinates3D& orientationVec)
        : head(headPos, WormPartType::Head),
        tail(tailPos, WormPartType::Tail),
        body{ WormPart(bodyPos, WormPartType::Body) },
        orientation(orientationVec)
    {
    }

    // Add methods for movement, growth, etc. as needed
};