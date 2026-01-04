// Module implementation for the SBlock class.

module sblock;

import block;
import position;
import <vector>;

// Set the S-block shape according to the current rotation (0 or 1).
// We use a fixed 3x3 local bounding box:
static void setSShape(std::vector<Position> &cells, int rotation) {
    cells.clear();
    if (rotation == 0) {
        // horizontal:
        //   . . .
        //   . S S
        //   S S .
        cells = {
            Position{1, 1}, Position{1, 2},
            Position{2, 0}, Position{2, 1}
        };
    } else {
        // vertical:
        //   S . .
        //   S S .
        //   . S .
        cells = {
            Position{0, 0},
            Position{1, 0}, Position{1, 1},
            Position{2, 1}
        };
    }
}

SBlock::SBlock(int id, int level)
    : Block{id, 'S', level}
{
    rotation = 0;
    setSShape(cells, rotation);
}

void SBlock::rotateCW() {
    rotation = (rotation + 1) % 2;
    setSShape(cells, rotation);
}

void SBlock::rotateCCW() {
    rotation = (rotation + 1) % 2;
    setSShape(cells, rotation);
}
