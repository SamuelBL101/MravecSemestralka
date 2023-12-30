//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_WORLD_H
#define MRAVECSEMESTRALKA_WORLD_H

#include "Block.h"
#include "Ant.h"

class World {
private:
    Block **map;
    int width;
    int height;
public:
    World(int width, int height);

    int getWidth() const;

    int getHeight() const;

    Block *getBlock(int x, int y);

    void setBlock(int x, int y, BlockType blockType);

    sf::RectangleShape** getRectMap(float size);

    void setBlockType(int x, int y, BlockType blockType);
};


#endif //MRAVECSEMESTRALKA_WORLD_H
