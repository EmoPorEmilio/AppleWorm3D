#include "CubeGrid.h"
#include <stdexcept>
#include <cmath>

CubeGrid::CubeGrid(int initialSize) : gridSize(initialSize) {
    if (initialSize < 0) {
        this->gridSize = 0;
        return;
    }
    grid.reserve(this->gridSize);

    for (int x = 0; x < this->gridSize; ++x) {
        std::vector<std::vector<CubeGridElement*>> plane;
        plane.reserve(this->gridSize);

        for (int y = 0; y < this->gridSize; ++y) {
            std::vector<CubeGridElement*> row;
            row.reserve(this->gridSize);

            for (int z = 0; z < this->gridSize; ++z) {
                row.push_back(new CubeGridElement(Vector3(x, y, z)));
            }
            plane.push_back(std::move(row));
        }
        grid.push_back(std::move(plane));
    }
}

GameObject* CubeGrid::getObject(Vector3 position) {
	return grid[position.x][position.y][position.z]->getObject();
}

void CubeGrid::setObject(Vector3 position, GameObject* object) {
	grid[position.x][position.y][position.z]->setObject(object);
}

CubeGrid::~CubeGrid() {
    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            for (size_t z = 0; z < grid[x][y].size(); ++z) {
                delete grid[x][y][z];
            }
        }
    }
}

CubeGridElement* CubeGrid::at(Vector3 position) {
    return grid[position.x][position.y][position.z];
}