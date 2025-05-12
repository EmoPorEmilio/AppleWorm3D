#include "CubeGrid.h"

CubeGrid::CubeGrid(int size)
    : size(size),
    grid(size, std::vector<std::vector<CubeGridElement>>(
        size, std::vector<CubeGridElement>(size, CubeGridElement(0, 0, 0))))
{
    // Optionally call init() here
}

void CubeGrid::init() {
    for (int x = 0; x < size; ++x)
        for (int y = 0; y < size; ++y)
            for (int z = 0; z < size; ++z)
                grid[x][y][z] = CubeGridElement(x, y, z);
}

CubeGridElement& CubeGrid::at(int x, int y, int z) {
    return grid[x][y][z];
}