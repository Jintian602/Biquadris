// Module implementation for the JBlock class.

module jblock;

import block;
import position;
import <vector>;

// set the J-block shape according to the current rotation.
// We use a fixed 3×3 local bounding box with coordinates:
//   row: 0 (top) .. 2 (bottom)
//   col: 0 (left) .. 2 (right)
static void setJShape(std::vector<Position> &cells, int rotation) {
    cells.clear();
    if (rotation == 0) {
        // rotation = 0:
        //   . . .
        //   J J J   (row 1)
        //   J . .   (row 2)
        cells = {
            Position{1, 0}, Position{1, 1}, Position{1, 2},
            Position{2, 0}
        };
    } else if (rotation == 1) {
        // rotation = 1:
        //   . J .   (row 0)
        //   . J .   (row 1)
        //   J J .   (row 2)
        cells = {
            Position{0, 1},
            Position{1, 1},
            Position{2, 1}, Position{2, 0}
        };
    } else if (rotation == 2) {
        // rotation = 2:
        //   . . .   (row 0)
        //   . . J   (row 1)
        //   J J J   (row 2)
        cells = {
            Position{1, 2},
            Position{2, 0}, Position{2, 1}, Position{2, 2}
        };
    } else {
        // rotation = 3:
        //   J J .   (row 0)
        //   J . .   (row 1)
        //   J . .   (row 2)
        cells = {
            Position{0, 0}, Position{0, 1},
            Position{1, 0},
            Position{2, 0}
        };
    }
}

JBlock::JBlock(int id, int level)
    : Block{id, 'J', level}
{
    rotation = 0;
    setJShape(cells, rotation);
}

void JBlock::rotateCW() {
    rotation = (rotation + 1) % 4;
    setJShape(cells, rotation);
}

void JBlock::rotateCCW() {
    rotation = (rotation + 3) % 4;
    setJShape(cells, rotation);
}
