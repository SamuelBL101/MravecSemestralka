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

std::string Block::toString() {
    std::string result = std::to_string(this->x) + " " + std::to_string(this->y);
    return result;
}
