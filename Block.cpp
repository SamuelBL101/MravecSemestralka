//
// Created by olive on 30. 12. 2023.
//

#include "Block.h"

void Block::setBlockType(BlockType newBlockType) {
    this->blockType = newBlockType;
}

BlockType Block::getBlockType() {
    return this->blockType;
}

Block::Block() {
    this->blockType = WHITE;
}
