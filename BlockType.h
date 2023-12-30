//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_BLOCKTYPE_H
#define MRAVECSEMESTRALKA_BLOCKTYPE_H

#include <string>

enum BlockType {
    ANT,
    WHITE,
    BLACK
};

inline std::string to_string(BlockType blockType) {
    switch (blockType) {
        case ANT:
            return "ANT";
        case WHITE:
            return "WHITE";
        case BLACK:
            return "BLACK";
        default:
            return "UNKNOWN";
    }
}

#endif //MRAVECSEMESTRALKA_BLOCKTYPE_H
