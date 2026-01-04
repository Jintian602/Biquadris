// Module implementation for the OBlock class.

module oblock;

import block;
import position;
import <vector>;

OBlock::OBlock(int id, int level)
    : Block{id, 'O', level}
{
    cells.clear();
    cells.push_back(Position{0, 0});
    cells.push_back(Position{0, 1});
    cells.push_back(Position{1, 0});
    cells.push_back(Position{1, 1});
}

void OBlock::rotateCW() {}

void OBlock::rotateCCW() {}
