//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_ANT_H
#define MRAVECSEMESTRALKA_ANT_H

#include "Block.h"
#include "BlockType.h"

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
};


#endif //MRAVECSEMESTRALKA_ANT_H
