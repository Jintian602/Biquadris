// Module implementation for the TBlock class.

module tblock;

import block;
import position;
import <vector>;

// sets the T-block shape according to the
// current rotation state (0, 1, 2, or 3).
// Local coordinates: row 0..2 (top..bottom), col 0..2 (left..right)
static void setTShape(std::vector<Position> &cells, int rotation) {
    cells.clear();
    if (rotation == 0) {
        // rotation 0 (T pointing up):
        //   . . .   (row 0)
        //   T T T   (row 1)
        //   . T .   (row 2)
        cells = {
            Position{1, 0}, Position{1, 1}, Position{1, 2},
            Position{2, 1}
        };
    } else if (rotation == 1) {
        // rotation 1 (T pointing right):
        //   T . .   (row 0)
        //   T T .   (row 1)
        //   T . .   (row 2)
        cells = {
            Position{0, 0},
            Position{1, 0}, Position{1, 1},
            Position{2, 0}
        };
    } else if (rotation == 2) {
        // rotation 2 (T pointing down):
        //   . T .   (row 0)
        //   T T T   (row 1)
        //   . . .   (row 2)
        // shift down by 1 so bottom is row 2:
        //   . . .   (row 0)
        //   . T .   (row 1)
        //   T T T   (row 2)
        cells = {
            Position{1, 1},
            Position{2, 0}, Position{2, 1}, Position{2, 2}
        };
    } else {
        // rotation 3 (T pointing left):
        //   . T .   (row 0)
        //   T T .   (row 1)
        //   . T .   (row 2)
        cells = {
            Position{0, 1},
            Position{1, 0}, Position{1, 1},
            Position{2, 1}
        };
    }
}

TBlock::TBlock(int id, int level)
    : Block{id, 'T', level}
{
    rotation = 0;
    setTShape(cells, rotation);
}

void TBlock::rotateCW() {
    rotation = (rotation + 1) % 4;
    setTShape(cells, rotation);
}

void TBlock::rotateCCW() {
    rotation = (rotation + 3) % 4;
    setTShape(cells, rotation);
}
