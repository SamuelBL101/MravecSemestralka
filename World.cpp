//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include "World.h"

World::World(int width, int height) {
    this->width = width;
    this->height = height;


    map = new Block *[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new Block[width];
        for (int j = 0; j < width; j++) {
            map[i][j] = Block(WHITE);
            map[i][j].setXandY(i, j);
        }
    }
}

Block *World::getBlock(int x, int y) {
    return &map[y][x];
}

void World::setBlock(int x, int y, BlockType blockType) {
    this->map[y][x].setBlockType(blockType);
}

int World::getWidth() const {
    return this->width;
}

int World::getHeight() const {
    return this->height;
}

sf::RectangleShape **World::getRectMap(float size) {
    sf::RectangleShape **rectMap = new sf::RectangleShape *[height];
    for (int i = 0; i < height; ++i) {
        rectMap[i] = new sf::RectangleShape[width];
        for (int j = 0; j < width; ++j) {
            rectMap[i][j].setSize(sf::Vector2f(size, size));
            rectMap[i][j].setPosition(j * size, i * size);
            rectMap[i][j].setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
            rectMap[i][j].setOutlineColor(sf::Color::Blue);
            rectMap[i][j].setOutlineThickness(2);
        }
    }
    return rectMap;
}

void World::setBlockType(int x, int y, BlockType blockType) {
    this->map[y][x].setBlockType(blockType);
}



