#pragma once
#include "GameObject.h"

GameObject::GameObject(Vector3 position) : position(position), animating(false) {
}


void GameObject::SetPosition(Vector3 position) {
	this->position = position;
}

Vector3 GameObject::GetPosition() {
	return this->position;
}