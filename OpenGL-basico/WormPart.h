#pragma once
#include "GameObject.h"
#include "Vector3.h"
#include "Animator.h"

enum class WormPartType { Head, Body, Tail };

class WormPart : public GameObject {
private:
    Animator* anim;
public:
    WormPartType type;

    WormPart(const Vector3 pos, WormPartType type);
	void SetType(WormPartType type);

    void draw() override;
    void animatorMove(Vector3 newPosition);
    void updateAnimator(float deltaTime);
    bool isAnimating();
    void update(float deltaTime) override;
    Vector3 getAnimatorEndPosition();
};