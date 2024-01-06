//
// Created by olive on 30. 12. 2023.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <fstream>
#include <SFML/Graphics/Sprite.hpp>
#include "World.h"

World::World(int width, int height, int numberOfAnts, bool random, float size) {
    this->width = width;
    this->height = height;
    this->sizeOfBlock = size;

    this->ants = std::vector<Ant>();
    if (random) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }
    this->map = std::vector<std::vector<Block>>(height, std::vector<Block>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            if (random) {
                map[i][j] = Block(static_cast<BlockType>(std::rand() % 2));
            } else {
                map[i][j] = Block(WHITE);
            }
            map[i][j].setXandY(i, j);
        }
    }

    float scale = static_cast<float>(size) / 960;
    //Ant a = Ant(this->getBlock(width / 2, height/2), UP, true, this->sizeOfBlock);
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant a = Ant(this->getBlock(std::rand() % width, std::rand() % height), UP, true, this->sizeOfBlock);
        a.setColor(A_BLUE);
        a.scale(scale, scale);
        a.goTo(a.getX() * size, a.getY() * size);
        ants.push_back(a);
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

std::vector<std::vector<sf::RectangleShape>> World::getRectMap(float size) {
    std::vector<std::vector<sf::RectangleShape>> rectMap(height, std::vector<sf::RectangleShape>(width));

    for (int i = 0; i < height; ++i) {
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

void World::drawMap(sf::RenderWindow *window) {
    sf::VertexArray vertexArray = this->getVertexArray(this->sizeOfBlock);
    window->draw(vertexArray);

    this->collisionDetection();

    for (int i = 0; i < this->ants.size(); ++i) {
        window->draw(this->ants.at(i).getSprite());
        if (ants.at(i).getX() < 0 || ants.at(i).getX() >= this->width || ants.at(i).getY() < 0 ||
            ants.at(i).getY() >= this->height) {
            std::cout << "out of bounds" << std::endl;
            ants.erase(ants.begin() + i);
        }
    }
}

void World::addAnt(Ant ant) {
    this->ants.push_back(ant);
}

void World::move() {
    for (auto &ant: ants) {
        ant.move();
        ant.setCurrentBlock(this->getBlock(ant.getX(), ant.getY()));
        //std::cout << ant->toString() << std::endl;
    }

}

void World::saveToFile(std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    } else {
        std::cout << "File opened successfully" << std::endl;
    }
    file << this->width << " " << this->height << std::endl;
    /*
    for (auto &ant: *this->ants) {
        file << ant->toString() << std::endl;
    }
     */
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            file << this->map[i][j].getBlockType() << " ";
        }
        file << std::endl;
    }
    file.close();

}

void World::loadFromFile(std::string &fileName, int lower) {
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    } else {
        std::cout << "File opened successfully" << std::endl;
    }
    file >> this->width >> this->height;
    this->sizeOfBlock = lower / (width > height ? width : height);

    this->map = std::vector<std::vector<Block>>(height, std::vector<Block>(width));

    this->ants = std::vector<Ant>();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            int blockType;
            file >> blockType;
            map[i][j] = Block((BlockType) blockType);
            map[i][j].setXandY(i, j);
        }
    }
    file.close();
}

int World::getNumberOfAnts() {
    return this->ants.size();
}

World::World(std::string fileName, int numberOfAnts, float size) {
    this->sizeOfBlock = size;
    this->loadFromFile(fileName, size);
    float scale = static_cast<float>(this->sizeOfBlock) / 960;
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant a = Ant(this->getBlock(rand() % 10, rand() % 10), UP, true, this->sizeOfBlock);
        a.setColor(A_BLACK);
        a.scale(scale, scale);
        a.goTo(a.getX() * this->sizeOfBlock, a.getY() * this->sizeOfBlock);
        ants.push_back(a);
    }
}

void World::setAntsLogic(int logic) {
    this->logicOfAnts = logic;
}

void World::setAntColor(ColoredAnt color, int antIndex) {
    this->ants.at(antIndex).setColor(color);
}

void World::collisionDetection() {
    for (int i = 0; i < this->ants.size(); ++i) {
        for (int j = i + 1; j < this->ants.size(); ++j) {
            if (this->ants.at(i).getCurrentBlock() == this->ants.at(j).getCurrentBlock()) {
                std::cout << this->ants.size() << " collision ";
                switch (logicOfAnts) {
                    case 0:
                        ants.erase(ants.begin() + i);
                        break;
                    case 1:
                        ants.erase(ants.begin() + i);
                        ants.erase(ants.begin() + i) + 1;
                        break;
                    case 2:
                        ants.at(i).changeBehavior();
                        break;
                }
                std::cout << this->ants.size() << std::endl;
            }
        }
    }
}

bool World::isPaused() const {
    return paused;
}

void World::setPaused(bool paused) {
    World::paused = paused;
}

void World::setSizeOfBlock(float size) {
    this->sizeOfBlock = size;
}

sf::VertexArray World::getVertexArray(float size) {
    sf::VertexArray vertexArray(sf::Quads, 4 * height * width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Calculate the position of the current quad
            float x = j * size;
            float y = i * size;

            // Set the vertices of the quad
            vertexArray[4 * (i * width + j)].position = sf::Vector2f(x, y);
            vertexArray[4 * (i * width + j) + 1].position = sf::Vector2f(x + size, y);
            vertexArray[4 * (i * width + j) + 2].position = sf::Vector2f(x + size, y + size);
            vertexArray[4 * (i * width + j) + 3].position = sf::Vector2f(x, y + size);

            // Set the color based on the block type
            sf::Color fillColor = (map[i][j].getBlockType() == BLACK) ? sf::Color::Black : sf::Color::White;
            for (int k = 0; k < 4; ++k) {
                vertexArray[4 * (i * width + j) + k].color = fillColor;
            }
        }
    }

    return vertexArray;
}



