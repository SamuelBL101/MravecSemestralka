//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ant.h"

Ant::Ant(int x, int y, Block *currentBlock) {
    //auto *texture = new sf::Texture();

    if (!this->texture.loadFromFile("images/ant.png")) {
        std::cout << "Could not load enemy texture" << std::endl;
    } else {
        std::cout << "Loaded enemy texture" << std::endl;
    }
    this->sprite.setTexture(texture);
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

void Ant::move(Direction direction) {
    switch (direction) {
        case UP:
            this->texture.loadFromFile("images/ant.png");
            this->sprite.setTexture(this->texture);
            this->y--;
            break;
        case DOWN:
            this->texture.loadFromFile("images/antD.png");
            this->sprite.setTexture(this->texture);
            this->y++;
            break;
        case LEFT:
            this->texture.loadFromFile("images/antL.png");
            this->sprite.setTexture(this->texture);
            this->x--;
            break;
        case RIGHT:
            this->texture.loadFromFile("images/antR.png");
            this->sprite.setTexture(this->texture);
            this->x++;
            break;
        default:
            std::cout << "Unknown direction" << std::endl;
            break;
    }

}

void Ant::move() {
    switch (this->currentBlock->getBlockType()) {
        case WHITE:
            this->setCurrentBlockType(BLACK);
            this->direction = (Direction) ((this->direction + 1) % 4);
            //this->sprite.setRotation(this->sprite.getRotation() + 90);
            this->move(this->direction);
            this->goTo(this->x * 70.f, this->y * 70.f);
            break;
        case BLACK:
            this->setCurrentBlockType(WHITE);
            this->direction = static_cast<Direction>((this->direction - 1 + 4) % 4);
            this->move(this->direction);
            this->goTo(this->x * 70.f, this->y * 70.f);
            break;
        case ANT:
            break;
    }
}

std::string Ant::toString() {
    return "Ant: x: " + std::to_string(this->x) + " y: " + std::to_string(this->y) + " direction: " +
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
