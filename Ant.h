//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_ANT_H
#define MRAVECSEMESTRALKA_ANT_H

#include "Block.h"
#include "BlockType.h"
enum Direction {
    UP, DOWN, LEFT, RIGHT
};

class Ant {
private:
    int x;
    int y;
    Block *currentBlock;
public:
    Ant(int x, int y, Block *currentBlock);

    int getX();

    int getY();

    Block *getCurrentBlock();

    void setCurrentBlockType(BlockType newBlockType);

    void move(enum Direction direction);
};


#endif //MRAVECSEMESTRALKA_ANT_H
