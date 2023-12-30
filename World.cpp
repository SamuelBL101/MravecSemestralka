//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include "World.h"

World::World(int width, int height, int numberOfBlockX, int numberOfBlockY) {
    this->width = width;
    this->height = height;
    this->blockWidth = this->width / numberOfBlockX;
    this->blockHeight = this->height / numberOfBlockY;

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

void World::printWorld(sf::RenderWindow& window, Ant *ant) {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            // Draw the block
            sf::RectangleShape block(sf::Vector2f(static_cast<float>(this->blockWidth), static_cast<float>(this->blockHeight)));
            block.setPosition(i * blockWidth, j * blockHeight);
            block.setFillColor(map[i][j].getBlockType() == BLACK ? sf::Color::Black : sf::Color::White);
            window.draw(block);

            // Draw horizontal line
            sf::RectangleShape horizontalLine(sf::Vector2f(static_cast<float>(this->blockWidth), 1)); // Adjust the thickness
            horizontalLine.setPosition(i * blockWidth, (j + 1) * blockHeight - 1); // Adjust position to be just above the next block
            horizontalLine.setFillColor(sf::Color::Red);
            window.draw(horizontalLine);

            // Draw vertical line
            sf::RectangleShape verticalLine(sf::Vector2f(1, static_cast<float>(this->blockHeight))); // Adjust the thickness
            verticalLine.setPosition((i + 1) * blockWidth - 1, j * blockHeight); // Adjust position to be just to the left of the next block
            verticalLine.setFillColor(sf::Color::Red);
            window.draw(verticalLine);

        }
    }
    sf::Texture texture;
    if (!texture.loadFromFile("images/ant.png")) {
        std::cout << "Could not load enemy texture" << std::endl;
    } else {
        sf::Sprite image;
        image.setTexture(texture);
        float scaleX = static_cast<float>(blockWidth) / texture.getSize().x;
        float scaleY = static_cast<float>(blockHeight) / texture.getSize().y;
        image.setScale( scaleX, scaleY);
        image.setPosition(ant->getX() * blockWidth, ant->getY() * blockHeight);
        window.draw(image);
    }
}


