/**
 * @file cell.cc
 * @brief Interface for the Cell class (board cell representation)
 * 
 * This file defines the Cell class, which represents a single cell in the
 * game board grid. Each cell can be occupied by a block or empty.
 */

export module cell;

/**
 * @class Cell
 * @brief Represents a single cell in the game board
 * 
 * A Cell can be either occupied (by a block) or empty. When occupied,
 * it stores the block's symbol, ID, and bornLevel for scoring purposes.
 */
export class Cell {
    char symbol;      ///< Character symbol of the occupying block
    int blockId;      ///< ID of the occupying block (-1 if not from a block)
    int bornLevel;    ///< Level at which the occupying block was generated
    bool occupied;    ///< Whether this cell is occupied
    
public:
    /**
     * @brief Constructs an empty Cell
     */
    Cell();
    
    /**
     * @brief Checks if the cell is occupied
     * @return true if occupied, false if empty
     */
    bool isOccupied() const;
    
    /**
     * @brief Gets the symbol of the occupying block
     * @return Character symbol, or undefined if empty
     */
    char getSymbol() const;
    
    /**
     * @brief Gets the ID of the occupying block
     * @return Block ID, or -1 if not from a block
     */
    int getBlockId() const;
    
    /**
     * @brief Gets the bornLevel of the occupying block
     * @return Born level, or 0 if not from a block
     */
    int getBornLevel() const;
    
    /**
     * @brief Sets the cell to occupied with block information
     * @param sym Block symbol character
     * @param id Block ID (-1 for special cells like dropped cells)
     * @param level Born level of the block
     */
    void set(char sym, int id, int level);
    
    /**
     * @brief Sets the cell to empty
     * 
     * Clears all cell data and marks it as unoccupied.
     */
    void unset();
};


