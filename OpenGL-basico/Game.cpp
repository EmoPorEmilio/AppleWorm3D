#include "Game.h"

Game::Game(int gridSize)
    : score(0), step(0), grid(gridSize)
{
    init();
}

void Game::init() {
    grid.init();
}