//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_BLOCK_H
#define MRAVECSEMESTRALKA_BLOCK_H


#include "BlockType.h"

class Block {
private:
    BlockType blockType;
    int x;
    int y;
public:

    Block();

    Block(BlockType blockType) : blockType(blockType) {};

    BlockType getBlockType();

    void setBlockType(BlockType newBlockType);

    std::string toString();

    void setXandY(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int getX() {
        return this->x;
    }

    int getY() {
        return this->y;
    }

    void setX(int x);

    void setY(int y);
};


#endif //MRAVECSEMESTRALKA_BLOCK_H
