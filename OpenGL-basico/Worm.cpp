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

void Worm::calculateNewWormOrientation(WormCommand command) {
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

void Worm::moveForward() {
    Vector3 prevPosition = this->head->GetPosition();
    this->head->SetPosition(this->head->GetPosition() + this->orientationForward);
    for (auto& part : body) {
        Vector3 temp = part->GetPosition();
        part->SetPosition(prevPosition);
        prevPosition = temp;
    }
    this->tail->SetPosition(prevPosition);
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

void Worm::grow() {
	Vector3 head = this->head->GetPosition();
	WormPart* newBodyPart = new WormPart(head, WormPartType::Body);
	std::cout << "New head position:" << head.x + orientationForward.x << ", " << head.y + orientationForward.y << ", " << head.z + orientationForward.z << std::endl;
	std::cout << "New bodypart position: " << newBodyPart->GetPosition().x << ", " << newBodyPart->GetPosition().y << ", " << newBodyPart->GetPosition().z << std::endl;
    this->head = new WormPart(Vector3(head.x+orientationForward.x, head.y + orientationForward.y, head.z + orientationForward.z), WormPartType::Head);
	body.push_back(newBodyPart);
}