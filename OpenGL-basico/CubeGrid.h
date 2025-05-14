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

    CubeGridElement* at(Vector3 position);

    CubeGrid(const CubeGrid&) = delete;
    CubeGrid& operator=(const CubeGrid&) = delete;
};