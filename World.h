//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_WORLD_H
#define MRAVECSEMESTRALKA_WORLD_H

#include "Block.h"

class World {
private:
    Block** map;
    int width;
    int height;
public:
    World(int width, int height);

    int getWidth();

    int getHeight();

    Block* getBlock(int x, int y);

    void setBlock(int x, int y, BlockType blockType);

};


#endif //MRAVECSEMESTRALKA_WORLD_H
