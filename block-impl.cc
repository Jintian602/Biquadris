// Block module - implementation
module block;

import <vector>;
import <limits>;
import position;

Block::Block(int id, char sym, int level) : id{id}, symbol{sym}, bornLevel{level} {}

int Block::getId() const { return id; }
char Block::getSymbol() const { return symbol; }
int Block::getBornLevel() const { return bornLevel; }
const std::vector<Position>& Block::getCells() const { return cells; }
Position Block::getOrigin() const { return origin; }

void Block::getBoundingBox(int& minRow, int& maxRow, int& minCol, int& maxCol) const {
    minRow = std::numeric_limits<int>::max();
    maxRow = std::numeric_limits<int>::min();
    minCol = std::numeric_limits<int>::max();
    maxCol = std::numeric_limits<int>::min();
    
    for (const auto& cell : cells) {
        minRow = std::min(minRow, cell.row);
        maxRow = std::max(maxRow, cell.row);
        minCol = std::min(minCol, cell.col);
        maxCol = std::max(maxCol, cell.col);
    }
}

void Block::setOrigin(const Position& pos) { origin = pos; }


