// Level1 module - implementation
module level1;

import <cstdlib>;   // for std::rand
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


Level1::Level1() : Level{1} {
    // Seeder is handled globally (e.g., by main with std::srand(seed)).
    // Nothing special needed here.
}

std::shared_ptr<Block> Level1::generateBlock(int id) {
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
        // Fallback if something unexpected happens.
        return std::make_shared<IBlock>(id, levelNum);
    }
}

char Level1::generateBlock() {
    if (!randomMode) {
        // Deterministic fallback (used only if someone disables randomness).
        return 'I';
    }

    // Use std::rand() instead of <random> utilities.
    // Level1 probabilities:
    //   S, Z: 1/12 each
    //   I, J, L, O, T: 1/6 each  (i.e., appear twice)
    const char blocks[12] = {
        'S', 'Z',
        'I', 'I',
        'J', 'J',
        'L', 'L',
        'O', 'O',
        'T', 'T'
    };
    int choice = std::rand() % 12;
    return blocks[choice];
}

void Level1::setRandom(bool random) {
    randomMode = random;
}

void Level1::setScriptFile(const std::string & /*filename*/) {
    // Level1 ignores script files (only Level0 uses them).
}

