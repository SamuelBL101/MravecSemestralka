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
    std::vector<std::vector<Block>> map;
    int width;
    int height;
    std::vector<Ant> ants;
    int logicOfAnts = 0;
    bool paused = false;
    void collisionDetection();
    int sizeOfBlock = 0;

public:
    World(int width, int height, int numberOfAnts, bool random, float size);

    World(std::string fileName, int numberOfAnts, float size);

    void move();

    int getWidth() const;

    int getHeight() const;

    Block *getBlock(int x, int y);

    void setBlock(int x, int y, BlockType blockType);

    std::vector<std::vector<sf::RectangleShape>> getRectMap(float size);

    void drawMap(sf::RenderWindow *window);

    void addAnt(Ant ant);

    int getNumberOfAnts();

    void changeBlockType(int x, int y);

    void setBlockSize(int size);

    void loadFromFile(std::string& fileName, int lower);

    void saveToFile(std::string fileName);

    void setAntsLogic(int logic);

    void setAntColor(ColoredAnt color, int antIndex);

    bool isPaused() const;

    void setPaused(bool paused);

    void setSizeOfBlock(float size);

    sf::VertexArray getVertexArray(float size);

    float getSizeOfBlock() const;

    void changeAntBehaviour();

};


#endif //MRAVECSEMESTRALKA_WORLD_H
