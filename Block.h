//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_BLOCK_H
#define MRAVECSEMESTRALKA_BLOCK_H


#include "BlockType.h"

class Block {
private:
    BlockType blockType;
public:

    Block();


    Block(BlockType blockType): blockType(blockType) {};

    BlockType getBlockType();

    void setBlockType(BlockType newBlockType);
};


#endif //MRAVECSEMESTRALKA_BLOCK_H
