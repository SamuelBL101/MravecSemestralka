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


    map = new Block *[width];
    for (int i = 0; i < width; ++i) {
        map[i] = new Block[height];
        for (int j = 0; j < height; j++) {
            map[i][j] = Block(WHITE);
        }
    }
}

Block *World::getBlock(int x, int y) {
    return &map[x][y];
}

void World::setBlock(int x, int y, BlockType blockType) {
    this->map[x][y].setBlockType(blockType);
}

int World::getWidth() const {
    return this->width;
}

int World::getHeight() const {
    return this->height;
}

//void World::printWorld(sf::RenderWindow &window, Ant *ant) {
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; ++j) {
//            // Draw the block
//            sf::RectangleShape block(
//                    sf::Vector2f(static_cast<float>(this->blockWidth), static_cast<float>(this->blockHeight)));
//            block.setPosition(i * blockWidth, j * blockHeight);
//            block.setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
//            window.draw(block);
//
//            // Draw horizontal line
//            sf::RectangleShape horizontalLine(
//                    sf::Vector2f(static_cast<float>(this->blockWidth), 1)); // Adjust the thickness
//            horizontalLine.setPosition(i * blockWidth,
//                                       (j + 1) * blockHeight - 1); // Adjust position to be just above the next block
//            horizontalLine.setFillColor(sf::Color::Red);
//            window.draw(horizontalLine);
//
//            // Draw vertical line
//            sf::RectangleShape verticalLine(
//                    sf::Vector2f(1, static_cast<float>(this->blockHeight))); // Adjust the thickness
//            verticalLine.setPosition((i + 1) * blockWidth - 1,
//                                     j * blockHeight); // Adjust position to be just to the left of the next block
//            verticalLine.setFillColor(sf::Color::Red);
//            window.draw(verticalLine);
//
//        }
//    }
//
//    float scaleX = static_cast<float>(blockWidth) / ant->getSprite().getTexture()->getSize().x;
//    float scaleY = static_cast<float>(blockHeight) / ant->getSprite().getTexture()->getSize().y;
//
//    ant->scale(scaleX, scaleY);
//    ant->goTo(ant->getX() * blockWidth, ant->getY() * blockHeight);
//
//    window.draw(ant->getSprite());
//}

void World::printAnt(sf::RenderWindow &window, Ant *ant) {
    float scaleX = static_cast<float>(width) / ant->getSprite().getTexture()->getSize().x;
    float scaleY = static_cast<float>(height) / ant->getSprite().getTexture()->getSize().y;

    ant->scale(scaleX, scaleY);
    ant->goTo(ant->getX() * width, ant->getY() * height);

    window.draw(ant->getSprite());

}

void World::printWorld(sf::RenderWindow &window, Ant *ant) {
    sf::RectangleShape rectangles[2][2];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            rectangles[i][j].setSize(sf::Vector2f(50.f, 50.f));
            rectangles[i][j].setPosition(j * 50.f, i * 50.f);
            rectangles[i][j].setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
            window.draw(rectangles[i][j]);
        }
    }
}

Block **World::getMap() {
    return this->map;
}

sf::RectangleShape **World::getRectMap(float size) {
    sf::RectangleShape **rectMap = new sf::RectangleShape *[width];
    for (int i = 0; i < width; ++i) {
        rectMap[i] = new sf::RectangleShape[height];
        for (int j = 0; j < height; ++j) {
            rectMap[i][j].setSize(sf::Vector2f(size, size));
            rectMap[i][j].setPosition(j * size, i * size);
            rectMap[i][j].setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
            rectMap[i][j].setOutlineColor(sf::Color::Blue);
            rectMap[i][j].setOutlineThickness(2);
        }
    }
    return rectMap;
}



