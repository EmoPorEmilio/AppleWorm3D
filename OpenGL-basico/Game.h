#pragma once
#include "CubeGrid.h"

class Game {
private:
    int score;
    int step;   
    CubeGrid grid;

public:
    Game(int gridSize);
    void init();
};