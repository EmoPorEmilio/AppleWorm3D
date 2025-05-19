#pragma once
#include <vector>
#include "Worm.h"
#include "WormPart.h"
#include <stdio.h>  
#include <iostream>

Worm::Worm(WormPart* head,
    WormPart* body,
    WormPart* tail,
    Vector3 orientationForward,
    Vector3 orientationUp) :
    head(head),
    tail(tail), 
    orientationForward(orientationForward),
    orientationUp(orientationUp)
{
	this->body = std::vector<WormPart*>();
    this->body.push_back(body);
}

void Worm::draw() {  
    this->head->draw();  
    this->tail->draw();
    for (auto& part : body) {
        part->draw();
    }  
}

void Worm::updateNewWormOrientation(WormCommand command) {
    Vector3 forward = this->orientationForward;
    Vector3 up = this->orientationUp;
    if (command == WormCommand::MOVE_RIGHT) {
        this->orientationForward = forward.Cross(up);
    }
    if (command == WormCommand::MOVE_LEFT) {
        this->orientationForward = up.Cross(forward);
    }
    if (command == WormCommand::MOVE_UP) {
        this->orientationForward = up;
        this->orientationUp = forward.operator*(-1.0);
    }
    if (command == WormCommand::MOVE_DOWN) {
        this->orientationForward = up.operator*(-1.0);
        this->orientationUp = forward;
    }
}

Vector3 Worm::calculateWormPotentialNextPosition(Vector3 orientation) {
    Vector3 currentPos = this->head->GetPosition();
    return currentPos.operator+(orientation);
}

Vector3 Worm::getOrientationForward() {
    return this->orientationForward;
}

Vector3 Worm::calculateNewWormForwardOrientation(WormCommand command) {
    Vector3 forward = this->orientationForward;
    Vector3 up = this->orientationUp;
    if (command == WormCommand::MOVE_RIGHT) {
        return forward.Cross(up);
    }
    if (command == WormCommand::MOVE_LEFT) {
        return up.Cross(forward);
    }
    if (command == WormCommand::MOVE_UP) {
        return up;
    }
	if (command == WormCommand::MOVE_DOWN) {
        return up.operator*(-1.0);
	}
}

void Worm::moveForward() {
    Vector3 prevPosition = this->head->GetPosition();
    //this->head->SetPosition(this->getNextPosition());
    this->head->animatorMove(this->getNextPosition());
    for (auto& part : body) {
        Vector3 temp = part->GetPosition();
        part->animatorMove(prevPosition);
        //part->SetPosition(prevPosition);
        prevPosition = temp;
    }
    this->tail->animatorMove(prevPosition);
    //this->tail->SetPosition(prevPosition);
}

void Worm::updateAnimation(float deltaTime) {
    this->head->updateAnimator(deltaTime);
    for (auto& part : body) {
        part->updateAnimator(deltaTime);
    }
    this->tail->updateAnimator(deltaTime);
}

bool Worm::isAnimating() {
    bool res = this->head->isAnimating() || this->tail->isAnimating();
    for (auto& part : body) {
        res = res || part->isAnimating();
    }
    return res;
}

void Worm::fall() {
    Vector3 head = this->head->GetPosition();
    Vector3 tail = this->tail->GetPosition();
    this->head->SetPosition(Vector3(head.x, head.y - 1, head.z));
    this->tail->SetPosition(Vector3(tail.x, tail.y - 1, tail.z));
   
    for (auto& part : body) {
        Vector3 temp = part->GetPosition();
        part->SetPosition(Vector3(temp.x, temp.y - 1, temp.z));
    }
}

Vector3 Worm::getNextPosition() {
    return this->head->GetPosition() + this->orientationForward;
}

WormPart* Worm::grow() {
	Vector3 head = this->head->GetPosition();
	//WormPart* newHead = new WormPart(this->getNextPosition(), WormPartType::Head);
    WormPart* newHead = new WormPart(head, WormPartType::Head);
    newHead->animatorMove(this->getNextPosition());
	this->head->SetType(WormPartType::Body);
	this->body.insert(this->body.begin(), this->head);
	this->head = newHead;
	return newHead;
}

std::vector<GameObject*> Worm::getParts() {
	std::vector<GameObject*> parts;
	parts.push_back(this->head);
	for (auto& part : body) {
		parts.push_back(part);
	}
	parts.push_back(this->tail);
	return parts;
}