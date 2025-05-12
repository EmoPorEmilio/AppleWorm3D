#pragma once
#include <vector>
#include "CubeGridElement.h"

class CubeGrid {
private:
    int size;
    std::vector<std::vector<std::vector<CubeGridElement>>> grid;

public:
    CubeGrid(int size);
    void init();
    CubeGridElement& at(int x, int y, int z); // Accessor
};