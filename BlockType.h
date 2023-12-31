//
// Created by olive on 30. 12. 2023.
//

#ifndef MRAVECSEMESTRALKA_BLOCKTYPE_H
#define MRAVECSEMESTRALKA_BLOCKTYPE_H

#include <string>
/**
 * Enum for block type
 */
enum BlockType {
    WHITE = 0,
    BLACK = 1
};
/**
 * Convert block type to string
 * @param blockType
 * @return
 */
inline std::string to_string(BlockType blockType) {
    switch (blockType) {
        case WHITE:
            return "WHITE";
        case BLACK:
            return "BLACK";
        default:
            return "UNKNOWN";
    }
}

#endif //MRAVECSEMESTRALKA_BLOCKTYPE_H
