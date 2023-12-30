//
// Created by olive on 30. 12. 2023.
//

#include "Ant.h"

Ant::Ant(int x, int y, Block *currentBlock) {
    this->x = x;
    this->y = y;
    this->currentBlock = currentBlock;
}

int Ant::getX() {
    return this->x;
}

int Ant::getY() {
    return this->y;
}

Block *Ant::getCurrentBlock() {
    return this->currentBlock;
}

void Ant::setCurrentBlockType(BlockType newBlock) {
    this->currentBlock->setBlockType(newBlock);
}
