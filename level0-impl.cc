// Level0 module - implementation
module level0;

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
import <fstream>;
import <vector>;



Level0::Level0() : Level{0}, scriptFile{"sequence.txt"} {
    loadSequence();
}


void Level0::loadSequence() {
    sequence.clear();
    std::ifstream file{scriptFile};
    char c;
    while (file >> c) {
        sequence.push_back(c);
    }
    currentIndex = 0;
}


std::shared_ptr<Block> Level0::generateBlock(int id) {
    char type = generateBlock();  // obtain next scripted type

    if (type == 'I') {
        return std::make_shared<IBlock>(id, levelNum);
    }
    else if (type == 'J') {
        return std::make_shared<JBlock>(id, levelNum);
    }
    else if (type == 'L') {
        return std::make_shared<LBlock>(id, levelNum);
    }
    else if (type == 'O') {
        return std::make_shared<OBlock>(id, levelNum);
    }
    else if (type == 'S') {
        return std::make_shared<SBlock>(id, levelNum);
    }
    else if (type == 'T') {
        return std::make_shared<TBlock>(id, levelNum);
    }
    else if (type == 'Z') {
        return std::make_shared<ZBlock>(id, levelNum);
    }
    else {
        // fallback to I-block if invalid character appears
        return std::make_shared<IBlock>(id, levelNum);
    }
}

char Level0::generateBlock() {
    if (sequence.empty()) return 'I';
    char type = sequence[currentIndex];
    currentIndex = (currentIndex + 1) % sequence.size();
    return type;
}

void Level0::setRandom(bool random) {
    // Level 0 is deterministic; this method has no effect.
}


void Level0::setScriptFile(const std::string& filename) {
    scriptFile = filename;
    loadSequence();
}


