// Module implementation for the ZBlock class.

module zblock;

import block;
import position;
import <vector>;

// sets the Z-block shape according to the
// current rotation state (0 or 1).
// Local coordinates: row 0..2 (top..bottom), col 0..2 (left..right)
static void setZShape(std::vector<Position> &cells, int rotation) {
    cells.clear();
    if (rotation == 0) {
        // rotation = 0 (horizontal):
        //   . . .   (row 0)
        //   Z Z .   (row 1)
        //   . Z Z   (row 2)
        cells = {
            Position{1, 0}, Position{1, 1},
            Position{2, 1}, Position{2, 2}
        };
    } else {
        // rotation = 1 (vertical):
        //   . Z .   (row 0)
        //   Z Z .   (row 1)
        //   Z . .   (row 2)
        cells = {
            Position{0, 1},
            Position{1, 0}, Position{1, 1},
            Position{2, 0}
        };
    }
}

ZBlock::ZBlock(int id, int level)
    : Block{id, 'Z', level}
{
    rotation = 0;
    setZShape(cells, rotation);
}

void ZBlock::rotateCW() {
    rotation = (rotation + 1) % 2;
    setZShape(cells, rotation);
}

void ZBlock::rotateCCW() {
    rotation = (rotation + 1) % 2;
    setZShape(cells, rotation);
}
