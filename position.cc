/**
 * @file position.cc
 * @brief Interface for the Position struct (2D coordinate)
 * 
 * This file defines the Position struct, which represents a 2D coordinate
 * (row, column) in the game board grid.
 */

export module position;

/**
 * @struct Position
 * @brief Represents a 2D coordinate (row, column)
 * 
 * Position is used throughout the game to represent:
 * - Block positions on the board
 * - Cell coordinates in the grid
 * - Relative positions of cells within a block
 * 
 * Row 0 is at the top, column 0 is at the left.
 */
export struct Position {
    int row;    ///< Row coordinate (0 = top)
    int col;    ///< Column coordinate (0 = left)
    
    /**
     * @brief Constructs a Position
     * @param r Row coordinate (default: 0)
     * @param c Column coordinate (default: 0)
     */
    Position(int r = 0, int c = 0);
    
    /**
     * @brief Copy constructor (default)
     */
    Position(const Position&) = default;
    
    /**
     * @brief Assignment operator (default)
     */
    Position& operator=(const Position&) = default;
    
    /**
     * @brief Equality comparison
     * @param other Other position to compare
     * @return true if row and col are equal
     */
    bool operator==(const Position& other) const;
    
    /**
     * @brief Inequality comparison
     * @param other Other position to compare
     * @return true if row or col differ
     */
    bool operator!=(const Position& other) const;
    
    /**
     * @brief Addition operator
     * @param other Position to add
     * @return New position with summed coordinates
     * 
     * Useful for calculating absolute positions from relative offsets.
     */
    Position operator+(const Position& other) const;
};


