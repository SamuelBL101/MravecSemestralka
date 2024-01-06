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

inline std::string to_string(Direction direction) {
    switch (direction) {
        case UP:
            return "UP";
        case RIGHT:
            return "RIGHT";
        case DOWN:
            return "DOWN";
        default:
            return "LEFT";
    }
}

enum ColoredAnt {
    A_BLACK = 0, A_GREEN = 1, A_BLUE = 2, A_YELLOW = 3, A_RED = 4
};

inline std::string to_string(ColoredAnt coloredAnt) {
    switch (coloredAnt) {
        case A_BLACK:
            return "B";
        case A_GREEN:
            return "Green";
        case A_BLUE:
            return "Blue";
        case A_YELLOW:
            return "Yellow";
        case A_RED:
            return "Red";
        default:
            return "B";
    }
}


class Ant {
private:
    Block* currentBlock;
    Direction direction;
    sf::Sprite sprite;
    sf::Texture texture;
    bool isInverse;
    int x;
    int y;
    ColoredAnt color;
    float size;

public:
    Ant(Block *currentBlock, Direction direction, bool isInverse, float size);

    int getX();

    int getY();

    sf::Sprite getSprite();

    void scale(float scaleX, float scaleY);

    Block *getCurrentBlock();

    void setCurrentBlockType(BlockType newBlockType);

    void goTo(int x, int y);

    void move(enum Direction direction);

    void move();

    void setCurrentBlock(Block *newBlock);

    std::string toString();

    void changeBehavior();

    void setColor(ColoredAnt color);

    int getColor();
};


#endif //MRAVECSEMESTRALKA_ANT_H
