#pragma once
#include <vector>
#include "WormPart.h"
#include "Vector3.h"

class Worm {
public:
    WormPart head;
    WormPart tail;
    std::vector<WormPart> body;
    Vector3 orientation; // Now just a single direction

    Worm(const Vector3 headPos,
        const Vector3 bodyPos,
        const Vector3 tailPos,
        const Vector3 orientationVec)
        : head(headPos, WormPartType::Head),
        tail(tailPos, WormPartType::Tail),
        body{ WormPart(bodyPos, WormPartType::Body) },
        orientation(orientationVec)
    {
    }

    // Add methods for movement, growth, etc. as needed
};