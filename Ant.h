//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_ANT_H
#define MRAVECSEMESTRALKA_ANT_H

#include <SFML/Graphics/Sprite.hpp>
#include "Block.h"
#include "BlockType.h"

enum Direction {
    UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3
};

class Ant {
private:
    int x;
    int y;
    Block *currentBlock;
    Direction direction;
    sf::Sprite sprite;
public:
    Ant(int x, int y, Block *currentBlock);

    int getX();

    int getY();

    sf::Sprite getSprite();

    void scale(float scaleX, float scaleY);

    Block *getCurrentBlock();

    void setCurrentBlockType(BlockType newBlockType);

    void goTo(int x, int y);

    void move(enum Direction direction);

    void move();

    std::string toString();
};


#endif //MRAVECSEMESTRALKA_ANT_H
