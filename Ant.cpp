//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ant.h"

Ant::Ant(int x, int y, Block *currentBlock) {
    auto *texture = new sf::Texture();
    if (!texture->loadFromFile("images/ant.png")) {
        std::cout << "Could not load enemy texture" << std::endl;
    } else {
        std::cout << "Loaded enemy texture" << std::endl;
    }
    this->sprite.setTexture(*texture);
    this->x = x;
    this->y = y;
    this->currentBlock = currentBlock;
    this->direction = UP;
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

void Ant::move(enum Direction direction) {
    switch (direction) {
        case UP:
            this->y++;
            break;
        case DOWN:
            this->y--;
            break;
        case LEFT:
            this->x--;
            break;
        case RIGHT:
            this->x++;
            break;
    }

}

void Ant::move() {
    switch (this->currentBlock->getBlockType()) {
        case WHITE:
            this->direction = (Direction) ((this->direction + 1) % 4);
            //this->move(this->direction);
            this->sprite.move(20,20);
            this->currentBlock->setBlockType(BLACK);

            sprite.setOrigin(0.f, 960.f);
            this->sprite.setRotation(90.f);
            break;
        case BLACK:
            this->direction = (Direction) ((this->direction - 1) % 4);
            this->currentBlock->setBlockType(WHITE);
            break;
        case ANT:
            break;
    }
}

std::string Ant::toString() {
    return "Ant: x: " + std::to_string(this->x) + " y: " + std::to_string(this->y) + " direction: " +
           std::to_string(this->direction);
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
