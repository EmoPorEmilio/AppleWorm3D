#pragma once
#include <vector>
#include "WormPart.h"
#include "Vector3.h"

enum WormCommand {
    MOVE_FORWARD,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT
};

class Worm {
public:
    WormPart* head;
    WormPart* tail;
    std::vector<WormPart*> body;
    Vector3 orientationForward;
    Vector3 orientationUp;

    Worm(WormPart* head,
        WormPart* body,
        WormPart* tail,
        Vector3 orientationForward,
        Vector3 orientationUp);

    void draw();

	void updateNewWormOrientation(WormCommand command);
    Vector3 calculateNewWormForwardOrientation(WormCommand command);
    Vector3 calculateWormPotentialNextPosition(Vector3 orientation);
    Vector3 getOrientationForward();
    void moveForward();
    Vector3 getNextPosition();
	std::vector<GameObject*> getParts();
    void fall();
    WormPart* grow();
};