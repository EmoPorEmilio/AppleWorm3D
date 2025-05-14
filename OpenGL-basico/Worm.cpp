#pragma once
#include <vector>
#include "WormPart.h"

class Worm {
public:
    WormPart head;
    WormPart tail;
    std::vector<WormPart> body;
    std::vector<Vector3> orientation;

    Worm(const Vector3 headPos,
        const Vector3 bodyPos,
        const Vector3 tailPos,
        const std::vector<Vector3>& orientationVec)
        : head(headPos, WormPartType::Head),
        tail(tailPos, WormPartType::Tail),
        body{ WormPart(bodyPos, WormPartType::Body) },
        orientation(orientationVec)
    {
    }
    void draw() {  
       head.draw();  
       tail.draw();  
       for (auto& part : body) {
           part.draw();  
       }  
    }
};