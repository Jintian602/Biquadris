/**
 * @file board.cc
 * @brief Interface for the Board class (game board representation)
 * 
 * This file defines the Board class, which represents the game board in Biquadris.
 * The board is a grid of cells where blocks are placed. It handles block placement,
 * row clearing, and provides access to cell states.
 */

export module board;

import <vector>;
import <memory>;
import <utility>;
import <map>;
import cell;
import block;
import position;

/**
 * @class Board
 * @brief Represents the game board grid where blocks are placed
 * 
 * The Board class manages a 18x11 grid of cells. It provides methods to:
 * - Check if blocks can be placed at specific positions
 * - Place and remove blocks
 * - Clear full rows and track removed blocks for scoring
 * - Handle special effects like blind mode
 * 
 * Note: Board does NOT inherit from Subject. Observers are attached to Player
 * objects, which access Board through getBoard().
 */
export class Board {
public:
    /**
     * @struct ClearRowsResult
     * @brief Structure to hold the result of clearing full rows
     * 
     * This structure is returned by clearFullRowsWithBlockInfo() to provide
     * information needed for scoring calculations. It contains the number of
     * rows cleared and the bornLevel values of blocks that were completely
     * removed (all cells cleared).
     * 
     * Note: Must be public because it's used as a return type for public method.
     */
    struct ClearRowsResult {
        int rowsCleared;                           ///< Number of full rows that were cleared
        std::vector<int> removedBlockBornLevels;   ///< bornLevel values of completely removed blocks
        
        /**
         * @brief Default constructor
         * 
         * Initializes with 0 rows cleared and empty removed blocks list.
         */
        ClearRowsResult() : rowsCleared(0) {}
        
        /**
         * @brief Constructor with parameters
         * @param rows Number of rows cleared
         * @param levels Vector of bornLevel values for removed blocks
         */
        ClearRowsResult(int rows, const std::vector<int>& levels) 
            : rowsCleared(rows), removedBlockBornLevels(levels) {}
    };
    
    /**
     * @brief Constructs a new empty Board
     * 
     * Initializes an 18x11 grid of empty cells.
     */
    Board();
    
    /**
     * @brief Gets the number of rows in the board
     * @return Number of rows (18)
     */
    int getRows() const;
    
    /**
     * @brief Gets the number of columns in the board
     * @return Number of columns (11)
     */
    int getCols() const;
    
    /**
     * @brief Checks if a block can be placed at the given position
     * @param block The block to check
     * @param pos The position to check (top-left corner of block)
     * @return true if the block can be placed, false otherwise
     * 
     * Checks if all cells of the block would be within bounds and not
     * overlapping with existing placed blocks.
     */
    bool canPlace(const Block& block, const Position& pos) const;
    
    /**
     * @brief Places a block on the board at the given position
     * @param block The block to place
     * @param pos The position to place at (top-left corner of block)
     * 
     * Marks all cells occupied by the block with the block's symbol,
     * ID, and bornLevel. Does not check if placement is valid - use
     * canPlace() first.
     */
    void place(const Block& block, const Position& pos);
    
    /**
     * @brief Removes a block from the board at the given position
     * @param block The block to remove
     * @param pos The position where the block is located
     * 
     * Unmarks all cells occupied by the block, making them empty.
     */
    void remove(const Block& block, const Position& pos);
    
    /**
     * @brief Clears all full rows and returns the count
     * @return Number of rows cleared
     * 
     * This is a convenience method that calls clearFullRowsWithBlockInfo()
     * and returns only the row count. Use clearFullRowsWithBlockInfo() if
     * you need information about removed blocks for scoring.
     */
    int clearFullRows();
    
    /**
     * @brief Clears all full rows and returns detailed information
     * @return ClearRowsResult containing rows cleared and removed block info
     * 
     * Finds all completely filled rows, removes them, and shifts remaining
     * rows down. Also tracks which blocks were completely removed (all cells
     * cleared) and returns their bornLevel values for scoring purposes.
     */
    ClearRowsResult clearFullRowsWithBlockInfo();
    
    /**
     * @brief Drops a cell in the specified column
     * @param col Column index (0-10)
     * 
     * Places a '*' cell at the bottommost empty position in the column.
     * Used for special effects or testing.
     */
    void drop(int col);
    
    /**
     * @brief Gets the character representation of a cell
     * @param row Row index (0-17)
     * @param col Column index (0-10)
     * @return Character representing the cell (' ' for empty, block symbol for occupied, '?' if blind)
     * 
     * Returns the symbol of the block occupying the cell, or ' ' if empty.
     * If blind effect is active and the cell is in the blind area (rows 3-12,
     * columns 3-9), returns '?' instead.
     */
    char getCell(int row, int col) const;
    
    /**
     * @brief Toggles the blind effect on/off
     * 
     * When blind is active, cells in rows 3-12 and columns 3-9 display
     * as '?' instead of their actual symbols. This affects getCell() output.
     */
    void toggleBlind();
    
    /**
     * @brief Sets the blind effect state directly
     * @param enabled true to enable blind effect, false to disable
     * 
     * Directly sets the blind effect state without toggling.
     * Used for precise control over the blind effect timing.
     */
    void setBlind(bool enabled);
    
    /**
     * @brief Resets the board to empty state
     * 
     * Clears all cells, making the board empty. Blind effect state is preserved.
     */
    void reset();

private:
    static const int ROWS = 18;                    ///< Number of rows in the board
    static const int COLS = 11;                    ///< Number of columns in the board
    std::vector<std::vector<Cell>> grid;          ///< 2D grid of cells (ROWS x COLS)
    bool isBlind = false;                          ///< Blind effect flag (columns 3-9, rows 3-12)
};


