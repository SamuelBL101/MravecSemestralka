//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ant.h"
/**
 * Constructor
 * @param currentBlock
 * @param direction
 * @param isInverse
 * @param size
 */
Ant::Ant(Block *currentBlock, Direction direction, bool isInverse, float size) {
    this->color = A_BLUE;
    this->size = size;

    if (!this->texture.loadFromFile("images/ant" + to_string(this->color) + ".png")) {
        std::cout << "Could not load enemy texture" << std::endl;
    }
    this->currentBlock = currentBlock;
    this->isInverse = isInverse;

    this->x = currentBlock->getY();
    this->y = currentBlock->getX();

    this->sprite.setTexture(texture);
    this->direction = direction;
}
/**
 * Get x
 * @return
 */
int Ant::getX() {
    return this->x;
}
/**
 * Get y
 * @return
 */
int Ant::getY() {
    return this->y;
}
/**
 * Get current block
 * @return
 */
Block *Ant::getCurrentBlock() {
    return this->currentBlock;
}
/**
 * Get direction
 * @return
 */
void Ant::setCurrentBlockType(BlockType newBlock) {
    this->currentBlock->setBlockType(newBlock);
}
/**
 * Get direction
 * @return
 */
void Ant::move(Direction direction) {
    switch (direction) {
        case UP:
            this->texture.loadFromFile("images/ant" + to_string(this->color) + ".png");
            this->sprite.setTexture(this->texture);
            this->y--;
            break;
        case DOWN:
            this->texture.loadFromFile("images/ant" + to_string(this->color) + "D.png");
            this->sprite.setTexture(this->texture);
            this->y++;
            break;
        case LEFT:
            this->texture.loadFromFile("images/ant" + to_string(this->color) + "L.png");
            this->sprite.setTexture(this->texture);
            this->x--;
            break;
        case RIGHT:
            this->texture.loadFromFile("images/ant" + to_string(this->color) + "R.png");
            this->sprite.setTexture(this->texture);
            this->x++;
            break;
        default:
            std::cout << "Unknown direction" << std::endl;
            break;
    }

}

/**
 * Move ant
 */
void Ant::move() {
    if (!this->isInverse && this->currentBlock->getBlockType() == WHITE ||
        this->isInverse && this->currentBlock->getBlockType() == BLACK) {
        this->setCurrentBlockType(!this->isInverse ? BLACK : WHITE);
        this->direction = (Direction) ((this->direction + 1) % 4);
        this->move(this->direction);
        this->goTo(this->x * this->size, this->y * this->size);
    } else {
        this->setCurrentBlockType(this->isInverse ? BLACK : WHITE);
        this->direction = static_cast<Direction>((this->direction - 1 + 4) % 4);
        this->move(this->direction);
        this->goTo(this->x * this->size, this->y * this->size);

    }
//    switch (this->currentBlock->getBlockType()) {
//        case WHITE:
//            this->setCurrentBlockType(BLACK);
//            this->direction = (Direction) ((this->direction + 1) % 4);
//            this->move(this->direction);
//            this->goTo(this->x * 70.f, this->y * 70.f);
//            break;
//        case BLACK:
//            this->setCurrentBlockType(WHITE);
//            this->direction = static_cast<Direction>((this->direction - 1 + 4) % 4);
//            this->move(this->direction);
//            this->goTo(this->x * 70.f, this->y * 70.f);
//            break;
//        default:
//            std::cout << "Unknown block type" << std::endl;
//            break;
//    }
}

/**
 * Convert ant to string
 * @return
 */
std::string Ant::toString() {
    return "Ant: x: " + std::to_string(this->getX()) + " y: " +
           std::to_string(this->getY()) + " direction: " +
           to_string(this->direction);
}

/**
 * Get sprite
 * @return
 */
sf::Sprite Ant::getSprite() {
    return this->sprite;
}

/**
 * Scale ant
 * @param scaleX
 * @param scaleY
 */
void Ant::scale(float scaleX, float scaleY) {
    this->sprite.setScale(scaleX, scaleY);
}

/**
 * Go to position
 * @param x
 * @param y
 */
void Ant::goTo(int x, int y) {
    this->sprite.setPosition(x, y);
}

/**
 * Set current block
 * @param newBlock
 */
void Ant::setCurrentBlock(Block *newBlock) {
    this->currentBlock = newBlock;
}

/**
 * Change behavior
 */
void Ant::changeBehavior() {
    this->isInverse = !this->isInverse;
}

/**
 * Set color
 * @param color
 */
void Ant::setColor(ColoredAnt color) {
    this->color = color;
}

/**
 * Get color
 * @return
 */
int Ant::getColor() {
    return this->color;
}
