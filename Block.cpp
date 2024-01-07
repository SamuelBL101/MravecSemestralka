//
// Created by olive on 30. 12. 2023.
//

#include "Block.h"
/**
 * Set block type
 * @param newBlockType
 */
void Block::setBlockType(BlockType newBlockType) {
    this->blockType = newBlockType;
}
/**
 * Get block type
 * @return
 */
BlockType Block::getBlockType() {
    return this->blockType;
}
/**
 * Constructor
 */
Block::Block() {
    this->blockType = WHITE;
}
/**
 * Set x
 * @param x
 */
void Block::setX(int x) {
    this->x = x;
}
/**
 * Set y
 * @param y
 */
void Block::setY(int y) {
    this->y = y;
}
/**
 * Convert block to string
 * @return
 */
std::string Block::toString() {
    std::string result = std::to_string(this->x) + " " + std::to_string(this->y);
    return result;
}
