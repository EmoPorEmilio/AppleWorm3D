#pragma once
#include <vector>
#include "WormPart.h"
#include "Coordinates3D.h"

class Worm {
public:
    WormPart head;
    WormPart tail;
    std::vector<WormPart> body;
    std::vector<Coordinates3D> orientation;

    Worm(const Coordinates3D& headPos,
        const Coordinates3D& bodyPos,
        const Coordinates3D& tailPos,
        const std::vector<Coordinates3D>& orientationVec)
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