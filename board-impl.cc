// Board module - implementation
module board;

import <vector>;
import <memory>;
import <map>;
import <utility>;
import <algorithm>;
import cell;
import block;
import position;

Board::Board() {
    grid.resize(ROWS);
    for (auto& row : grid) {
        row.resize(COLS);
    }
}

int Board::getRows() const { return ROWS; }
int Board::getCols() const { return COLS; }

bool Board::canPlace(const Block& block, const Position& pos) const {
    for (const auto& cell : block.getCells()) {
        int r = pos.row + cell.row;
        int c = pos.col + cell.col;
        
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return false;
        if (grid[r][c].isOccupied()) return false;
    }
    return true;
}

void Board::place(const Block& block, const Position& pos) {
    for (const auto& cell : block.getCells()) {
        int r = pos.row + cell.row;
        int c = pos.col + cell.col;
        
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
            grid[r][c].set(block.getSymbol(), block.getId(), block.getBornLevel());
        }
    }
}

void Board::remove(const Block& block, const Position& pos) {
    for (const auto& cell : block.getCells()) {
        int r = pos.row + cell.row;
        int c = pos.col + cell.col;
        
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
            grid[r][c].unset();
        }
    }
}

int Board::clearFullRows() {
    auto result = clearFullRowsWithBlockInfo();
    return result.rowsCleared;
}

Board::ClearRowsResult Board::clearFullRowsWithBlockInfo() {
    // Track block cell counts before clearing
    std::map<int, int> blockCellCounts;  // blockId -> cell count
    std::map<int, int> blockBornLevels;  // blockId -> bornLevel
    
    // Count cells for each block
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c].isOccupied()) {
                int blockId = grid[r][c].getBlockId();
                if (blockId >= 0) {  // Valid block ID
                    blockCellCounts[blockId]++;
                    if (blockBornLevels.find(blockId) == blockBornLevels.end()) {
                        blockBornLevels[blockId] = grid[r][c].getBornLevel();
                    }
                }
            }
        }
    }
    
    // Find full rows
    int cleared = 0;
    std::vector<bool> isFullRow(ROWS, false);
    for (int r = ROWS - 1; r >= 0; --r) {
        bool full = true;
        for (int c = 0; c < COLS; ++c) {
            if (!grid[r][c].isOccupied()) {
                full = false;
                break;
            }
        }
        
        if (full) {
            isFullRow[r] = true;
            ++cleared;
        }
    }
    
    // Build new grid: keep non-full rows, add empty rows at top for cleared rows
    std::vector<std::vector<Cell>> newGrid;
    newGrid.reserve(ROWS);
    
    // Add empty rows at top for cleared rows
    for (int i = 0; i < cleared; ++i) {
        newGrid.push_back(std::vector<Cell>(COLS));
    }
    
    // Copy non-full rows
    for (int r = 0; r < ROWS; ++r) {
        if (!isFullRow[r]) {
            newGrid.push_back(grid[r]);
        }
    }
    
    // Replace old grid with new grid
    grid = std::move(newGrid);
    
    // Count cells for each block after clearing
    std::map<int, int> blockCellCountsAfter;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (grid[r][c].isOccupied()) {
                int blockId = grid[r][c].getBlockId();
                if (blockId >= 0) {
                    blockCellCountsAfter[blockId]++;
                }
            }
        }
    }
    
    // Find completely removed blocks (all cells cleared)
    // Only need bornLevel for scoring, not blockId
    std::vector<int> removedBlockBornLevels;
    for (const auto& [blockId, countBefore] : blockCellCounts) {
        int countAfter = blockCellCountsAfter[blockId];
        if (countAfter == 0) {
            // Block is completely removed - store its bornLevel for scoring
            removedBlockBornLevels.push_back(blockBornLevels[blockId]);
        }
    }
    
    return Board::ClearRowsResult(cleared, removedBlockBornLevels);
}

void Board::drop(int col) {
    // Find the topmost (lowest row number) occupied cell in the column
    int topmostOccupied = -1;
    for (int r = 0; r < ROWS; ++r) {
        if (grid[r][col].isOccupied()) {
            topmostOccupied = r;
            break;
        }
    }
    
    if (topmostOccupied == -1) {
        // Column is empty, place at the bottom
        grid[ROWS - 1][col].set('*', -1, 0);
    } else if (topmostOccupied > 0) {
        // Place one row above the topmost occupied cell
        grid[topmostOccupied - 1][col].set('*', -1, 0);
    }
    // If topmostOccupied == 0, column is full from top, cannot place
}

char Board::getCell(int row, int col) const {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return ' ';
    
    // Blind effect: columns 3-9, rows 3-12 are covered with '?'
    if (isBlind && row >= 3 && row <= 12 && col >= 3 && col <= 9) {
        return '?';
    }
    
    return grid[row][col].isOccupied() ? grid[row][col].getSymbol() : ' ';
}

void Board::toggleBlind() {
    isBlind = !isBlind;
}

void Board::setBlind(bool enabled) {
    isBlind = enabled;
}

void Board::reset() {
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.unset();
        }
    }
    // Reset blind effect state
    isBlind = false;
}



