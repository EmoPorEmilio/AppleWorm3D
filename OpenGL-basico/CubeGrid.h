#pragma once
#include <vector>
#include "CubeGridElement.h"
#include "Vector3.h"

class CubeGrid {
private:
    int gridSize;
    std::vector<std::vector<std::vector<CubeGridElement*>>> grid;

public:
    CubeGrid(int initialSize);
    ~CubeGrid();

    int getSize();
    CubeGridElement* at(Vector3 position);
    GameObject* getObject(Vector3 position);
	void setObject(Vector3 position, GameObject* object);
    CubeGrid(const CubeGrid&) = delete;
    CubeGrid& operator=(const CubeGrid&) = delete;
};