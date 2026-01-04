// Level4 module - implementation
module level4;

import level3;
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

Level4::Level4() : Level3{} {
    // Level4 inherits Level3's behavior, but needs levelNum = 4
    // Level3 constructor sets levelNum to 3, so we need to override it
    levelNum = 4;
}

// Level4 uses Level3's generateBlock methods, delegate to parent
std::shared_ptr<Block> Level4::generateBlock(int id) {
    return Level3::generateBlock(id);
}

char Level4::generateBlock() {
    return Level3::generateBlock();
}

void Level4::setRandom(bool random) {
    Level3::setRandom(random);
}

void Level4::setScriptFile(const std::string& filename) {
    Level3::setScriptFile(filename);
}

