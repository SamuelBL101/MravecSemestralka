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


class Ant {
private:
    Block *currentBlock;
    Direction direction;
    sf::Sprite sprite;
    sf::Texture texture;
    int x;
    int y;
public:
    Ant(Block *currentBlock, Direction direction);

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
};


#endif //MRAVECSEMESTRALKA_ANT_H
