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
    this->width = 70;
    this->height = 70;
    this->blockType = WHITE;
}

void Block::setX(int x) {
    this->x = x;
}

void Block::setY(int y) {
    this->y = y;
}

int Block::getWidth() const {
    return width;
}

void Block::setWidth(int width) {
    Block::width = width;
}

int Block::getHeight() const {
    return height;
}

void Block::setHeight(int height) {
    Block::height = height;
}

std::string Block::toString() {
    std::string result = std::to_string(this->x) + " " + std::to_string(this->y);
    return result;
}
