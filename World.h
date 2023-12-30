//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_WORLD_H
#define MRAVECSEMESTRALKA_WORLD_H

#include "Block.h"
#include "Ant.h"
class World {
private:
    Block** map;
    int width;
    int height;
    int blockWidth;
    int blockHeight;
public:
    World(int width, int height, int numberOfBlockX, int numberOfBlockY);
    int getWidth() const;

    int getHeight() const;

    Block* getBlock(int x, int y);

    void setBlock(int x, int y, BlockType blockType);

    void printWorld(sf::RenderWindow &window, Ant *ant);
};


#endif //MRAVECSEMESTRALKA_WORLD_H
