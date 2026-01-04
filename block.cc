/**
 * @file block.cc
 * @brief Interface for the Block class (abstract block interface)
 * 
 * This file defines the abstract Block class, which represents a Tetris block
 * (tetromino) in Biquadris. Each block consists of 4 cells arranged in a pattern.
 * Blocks can be rotated and positioned on the game board.
 */

export module block;

import <vector>;
import position;

/**
 * @class Block
 * @brief Abstract base class representing a Tetris block (tetromino)
 * 
 * The Block class represents a tetromino piece that can be placed on the board.
 * Each block consists of 4 cells arranged in a specific pattern (I, J, L, O, S, T, Z).
 * 
 * Blocks can be:
 * - Rotated clockwise or counter-clockwise
 * - Positioned on the board using an origin point
 * - Identified by a unique ID and symbol
 * 
 * Concrete block types (IBlock, JBlock, etc.) inherit from this class and
 * implement the rotation logic specific to each block shape.
 */
export class Block {
protected:
    int id;                              ///< Unique identifier for this block
    std::vector<Position> cells;          ///< Relative positions of cells (relative to origin)
    Position origin;                     ///< Origin position (top-left reference point)
    char symbol;                         ///< Character symbol representing this block type
    int bornLevel;                       ///< Level at which this block was generated
    int rotation = 0;                    ///< Current rotation state (0-3)
    
public:
    /**
     * @brief Constructs a new Block
     * @param id Unique identifier for this block
     * @param sym Character symbol (I, J, L, O, S, T, Z)
     * @param level Level at which this block was generated
     */
    Block(int id, char sym, int level);
    
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~Block() = default;
    
    /**
     * @brief Gets the block's unique ID
     * @return Block ID
     */
    int getId() const;
    
    /**
     * @brief Gets the block's symbol character
     * @return Character symbol (I, J, L, O, S, T, Z)
     */
    char getSymbol() const;
    
    /**
     * @brief Gets the level at which this block was generated
     * @return Born level (0-4)
     * 
     * Used for scoring when blocks are completely removed.
     */
    int getBornLevel() const;
    
    /**
     * @brief Gets the relative positions of all cells
     * @return Const reference to vector of cell positions (relative to origin)
     */
    const std::vector<Position>& getCells() const;
    
    /**
     * @brief Gets the origin position
     * @return Origin position
     */
    Position getOrigin() const;
    
    /**
     * @brief Gets the bounding box of the block
     * @param minRow Output: minimum row coordinate
     * @param maxRow Output: maximum row coordinate
     * @param minCol Output: minimum column coordinate
     * @param maxCol Output: maximum column coordinate
     * 
     * Calculates the bounding rectangle that contains all cells of the block.
     * Used for rotation position adjustments.
     */
    void getBoundingBox(int& minRow, int& maxRow, int& minCol, int& maxCol) const;
    
    /**
     * @brief Rotates the block clockwise
     * 
     * Pure virtual method - must be implemented by concrete block types.
     * Each block type has its own rotation pattern.
     */
    virtual void rotateCW() = 0;
    
    /**
     * @brief Rotates the block counter-clockwise
     * 
     * Pure virtual method - must be implemented by concrete block types.
     * Each block type has its own rotation pattern.
     */
    virtual void rotateCCW() = 0;
    
    /**
     * @brief Sets the origin position of the block
     * @param pos New origin position
     * 
     * Changes where the block is positioned on the board. All cell
     * positions are relative to this origin.
     */
    void setOrigin(const Position& pos);
};


