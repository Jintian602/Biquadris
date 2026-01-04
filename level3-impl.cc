// Level3 module - implementation
module level3;

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

Level3::Level3() : Level{3} {}

std::shared_ptr<Block> Level3::generateBlock(int id) {
    char type = generateBlock();

    if (type == 'I') return std::make_shared<IBlock>(id, levelNum);
    else if (type == 'J') return std::make_shared<JBlock>(id, levelNum);
    else if (type == 'L') return std::make_shared<LBlock>(id, levelNum);
    else if (type == 'O') return std::make_shared<OBlock>(id, levelNum);
    else if (type == 'S') return std::make_shared<SBlock>(id, levelNum);
    else if (type == 'T') return std::make_shared<TBlock>(id, levelNum);
    else if (type == 'Z') return std::make_shared<ZBlock>(id, levelNum);
    else return std::make_shared<IBlock>(id, levelNum);
}

char Level3::generateBlock() {
    if (!randomMode) return 'I';

    const char blocks[9] = {
        'S','Z','S','Z',  // 4/9 for S,Z
        'I','J','L','O','T'
    };

    int choice = rand() % 9;
    return blocks[choice];
}

void Level3::setRandom(bool random) {
    randomMode = random;
}

void Level3::setScriptFile(const std::string &) {}
