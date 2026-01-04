// Level2 module - implementation
module level2;

import <cstdlib>;   
import level;
import block;
import iblock;
import jblock;
import lblock;
import oblock;
import sblock;
import tblock;
import zblock;
import <memory>;
import <string>;


Level2::Level2() : Level{2} {
    // RNG seeding is handled elsewhere (e.g., main).
}

std::shared_ptr<Block> Level2::generateBlock(int id) {
    char type = generateBlock();

    if (type == 'I') {
        return std::make_shared<IBlock>(id, levelNum);
    } else if (type == 'J') {
        return std::make_shared<JBlock>(id, levelNum);
    } else if (type == 'L') {
        return std::make_shared<LBlock>(id, levelNum);
    } else if (type == 'O') {
        return std::make_shared<OBlock>(id, levelNum);
    } else if (type == 'S') {
        return std::make_shared<SBlock>(id, levelNum);
    } else if (type == 'T') {
        return std::make_shared<TBlock>(id, levelNum);
    } else if (type == 'Z') {
        return std::make_shared<ZBlock>(id, levelNum);
    } else {
        return std::make_shared<IBlock>(id, levelNum);
    }
}

char Level2::generateBlock() {
    if (!randomMode) {
        return 'I';
    }

    // Level2: all 7 blocks equally likely.
    const char blocks[7] = {'I', 'J', 'L', 'O', 'S', 'T', 'Z'};
    int choice = std::rand() % 7;
    return blocks[choice];
}

void Level2::setRandom(bool random) {
    randomMode = random;
}

void Level2::setScriptFile(const std::string & /*filename*/) {
    // Level2 does not use script files.
}

