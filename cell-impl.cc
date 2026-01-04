// Cell module - implementation
module;

module cell;

Cell::Cell() : symbol{'?'}, blockId{-1}, bornLevel{0}, occupied{false} {}

bool Cell::isOccupied() const { return occupied; }
char Cell::getSymbol() const { return symbol; }
int Cell::getBlockId() const { return blockId; }
int Cell::getBornLevel() const { return bornLevel; }

void Cell::set(char sym, int id, int level) {
    symbol = sym;
    blockId = id;
    bornLevel = level;
    occupied = true;
}

void Cell::unset() {
    symbol = '?';
    blockId = -1;
    bornLevel = 0;
    occupied = false;
}


