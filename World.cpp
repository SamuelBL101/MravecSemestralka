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

World::World(int width, int height, int numberOfAnts) {
    this->width = width;
    this->height = height;

    this->ants = new std::vector<Ant *>;

    map = new Block *[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new Block[width];
        for (int j = 0; j < width; j++) {
            map[i][j] = Block(WHITE);
            map[i][j].setXandY(i, j);
        }
    }

    float scale = static_cast<float>(70.f) / 960;
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant *a = new Ant(this->getBlock(width/2 + i, height/2 + i), UP);
        a->scale(scale, scale);
        a->goTo(a->getX() * 70.f, a->getY() * 70.f);
        ants->push_back(a);
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

void World::drawMap(sf::RenderWindow *window) {
    sf::RectangleShape **rectMap = this->getRectMap(70.f);
    for (int i = 0; i < this->getHeight(); ++i) {
        for (int j = 0; j < this->getWidth(); ++j) {
            window->draw(rectMap[i][j]);
        }
    }

    for (int i = 0; i < this->ants->size(); ++i) {
        window->draw(this->ants->at(i)->getSprite());
        if (i < this->ants->size() - 1) {
            if (ants->at(i)->getCurrentBlock() == ants->at(i + 1)->getCurrentBlock()) {
                std::cout << "collision" << std::endl;
                ants->erase(ants->begin() + i);
            }
        }
        if (ants->at(i)->getX() < 0 || ants->at(i)->getX() >= this->width || ants->at(i)->getY() < 0 ||
            ants->at(i)->getY() >= this->height) {
            std::cout << "out of bounds" << std::endl;
            ants->erase(ants->begin() + i);
        }
    }

}

void World::addAnt(Ant *ant) {
    this->ants->push_back(ant);
}

void World::move() {
    for (auto &ant: *ants) {
        ant->move();
        ant->setCurrentBlock(this->getBlock(ant->getX(), ant->getY()));
        std::cout << ant->toString() << std::endl;
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

int World::getNumberOfAnts() {
    return this->ants->size();
}

World::World(std::string fileName, int numberOfAnts) {
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << fileName << std::endl;
        return;
    } else {
        std::cout << "File opened successfully" << std::endl;
    }
    file >> this->width >> this->height;
    this->ants = new std::vector<Ant *>;
    map = new Block *[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new Block[width];
        for (int j = 0; j < width; j++) {
            int blockType;
            file >> blockType;
            map[i][j] = Block((BlockType) blockType);
            map[i][j].setXandY(i, j);
        }
    }
    file.close();
    float scale = static_cast<float>(70.f) / 960;
    for (int i = 0; i < numberOfAnts; ++i) {
        Ant *a = new Ant(this->getBlock(width/2 + i, height/2 + i), UP);
        a->scale(scale, scale);
        a->goTo(a->getX() * 70.f, a->getY() * 70.f);
        ants->push_back(a);
    }
}



