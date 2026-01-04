// lblock-impl.cc
module lblock;

import block;
import position;
import <vector>;

// set the L-block shape according to the current rotation.
// We use a fixed 3×3 local bounding box with coordinates:
//   row: 0 (top) .. 2 (bottom)
//   col: 0 (left) .. 2 (right)
static void setLShape(std::vector<Position> &cells, int rotation) {
    cells.clear();

    if (rotation == 0) {
        // rotation = 0:
        //   . . .   (row 0)
        //   L L L   (row 1)
        //   . . L   (row 2)
        cells = {
            Position{1, 0}, Position{1, 1}, Position{1, 2},
            Position{2, 2}
        };
    } else if (rotation == 1) {
        // rotation = 1:
        //   . L .   (row 0)
        //   . L .   (row 1)
        //   . L L   (row 2)
        cells = {
            Position{0, 1},
            Position{1, 1},
            Position{2, 1}, Position{2, 2}
        };
    } else if (rotation == 2) {
        // rotation = 2:
        //   . . .   (row 0)
        //   L L L   (row 1)
        //   L . .   (row 2)
        cells = {
            Position{1, 0}, Position{1, 1}, Position{1, 2},
            Position{2, 0}
        };
    } else {
        // rotation = 3:
        //   L L .   (row 0)
        //   . L .   (row 1)
        //   . L .   (row 2)
        cells = {
            Position{0, 0}, Position{0, 1},
            Position{1, 1},
            Position{2, 1}
        };
    }
}

LBlock::LBlock(int id, int level)
    : Block{id, 'L', level}
{
    rotation = 0;
    setLShape(cells, rotation);
}

void LBlock::rotateCW() {
    rotation = (rotation + 1) % 4;
    setLShape(cells, rotation);
}

void LBlock::rotateCCW() {
    rotation = (rotation + 3) % 4;
    setLShape(cells, rotation);
}
