//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ant.h"

Ant::Ant(Block *currentBlock, Direction direction, bool isInverse) {
    this->color = A_BLUE;

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

void Ant::move(Direction direction) {
    std::cout << "color of ant is " << to_string(this->color) << std::endl;
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

void Ant::move() {
    if (!this->isInverse && this->currentBlock->getBlockType() == WHITE ||
        this->isInverse && this->currentBlock->getBlockType() == BLACK) {
        this->setCurrentBlockType(!this->isInverse ? BLACK : WHITE);
        this->direction = (Direction) ((this->direction + 1) % 4);
        this->move(this->direction);
        this->goTo(this->x * 70.f, this->y * 70.f);
    } else {
        this->setCurrentBlockType(this->isInverse ? BLACK : WHITE);
        this->direction = static_cast<Direction>((this->direction - 1 + 4) % 4);
        this->move(this->direction);
        this->goTo(this->x * 70.f, this->y * 70.f);

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

std::string Ant::toString() {
    return "Ant: x: " + std::to_string(this->getX()) + " y: " +
           std::to_string(this->getY()) + " direction: " +
           to_string(this->direction);
}

sf::Sprite Ant::getSprite() {
    return this->sprite;
}

void Ant::scale(float scaleX, float scaleY) {
    this->sprite.setScale(scaleX, scaleY);
}

void Ant::goTo(int x, int y) {
    this->sprite.setPosition(x, y);
}

void Ant::setCurrentBlock(Block *newBlock) {
    this->currentBlock = newBlock;
}

void Ant::changeBehavior() {
    this->isInverse = !this->isInverse;
}

void Ant::setColor(ColoredAnt color) {
    std::cout << "color is " << to_string(color) << std::endl;
    this->color = color;
    std::cout << "new color is " << to_string(this->color) << std::endl;
}
