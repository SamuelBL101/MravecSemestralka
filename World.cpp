//
// Created by olive on 30. 12. 2023.
//

#include "World.h"

World::World(int width, int height) {
    this->width = width;
    this->height = height;

    map = new Block *[width];
    for (int i = 0; i < width; ++i) {
        map[i] = new Block[height];
        for (int j = 0; j < height; j++) {
            map[i][j] = Block(WHITE);
        }
    }
}

Block *World::getBlock(int x, int y) {
    return &map[x][y];
}

void World::setBlock(int x, int y, BlockType blockType) {
    this->map[x][y].setBlockType(blockType);
}

int World::getWidth() {
    return this->width;
}

int World::getHeight() {
    return this->height;
}
