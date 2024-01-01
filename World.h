//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_WORLD_H
#define MRAVECSEMESTRALKA_WORLD_H

#include "Block.h"
#include "Ant.h"
#include <vector>

class World {
private:
    Block **map;
    int width;
    int height;
    std::vector<Ant*> *ants;
public:
    World(int width, int height, int numberOfAnts);

    World(std::string fileName, int numberOfAnts);

    void move();

    int getWidth() const;

    int getHeight() const;

    Block *getBlock(int x, int y);

    void setBlock(int x, int y, BlockType blockType);

    sf::RectangleShape** getRectMap(float size);

    void drawMap(sf::RenderWindow *window);

    void addAnt(Ant *ant);

    int getNumberOfAnts();

    void setBlockType(int x, int y, BlockType blockType);

    void saveToFile(std::string fileName);
};


#endif //MRAVECSEMESTRALKA_WORLD_H
