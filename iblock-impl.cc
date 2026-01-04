module iblock;

import block;
import position;
import <vector>;

IBlock::IBlock(int id, int level)
    : Block{id, 'I', level}
{
    rotation = 0;
    // rotation = 0 (horizontal):
    // row 0: I I I I
    cells.clear();
    cells.push_back(Position{0, 0});
    cells.push_back(Position{0, 1});
    cells.push_back(Position{0, 2});
    cells.push_back(Position{0, 3});
}

void IBlock::rotateCW() {
    rotation = (rotation + 1) % 2;
    cells.clear();
    if (rotation == 0) {
        // Horizontal:
        // row 0: I I I I
        cells.push_back(Position{0, 0});
        cells.push_back(Position{0, 1});
        cells.push_back(Position{0, 2});
        cells.push_back(Position{0, 3});
    } else {
        // Vertical (grow upwards to keep bottom fixed):
        //
        // row -3: I
        // row -2: I
        // row -1: I
        // row  0: I   ← bottom row still 0
        cells.push_back(Position{0, 0});
        cells.push_back(Position{-1, 0});
        cells.push_back(Position{-2, 0});
        cells.push_back(Position{-3, 0});
    }
}

void IBlock::rotateCCW() {
    // For a piece with 2 states, CW and CCW are the same toggle.
    rotation = (rotation + 1) % 2;
    cells.clear();
    if (rotation == 0) {
        // Horizontal again
        cells.push_back(Position{0, 0});
        cells.push_back(Position{0, 1});
        cells.push_back(Position{0, 2});
        cells.push_back(Position{0, 3});
    } else {
        // Vertical, same as above
        cells.push_back(Position{0, 0});
        cells.push_back(Position{-1, 0});
        cells.push_back(Position{-2, 0});
        cells.push_back(Position{-3, 0});
    }
}
