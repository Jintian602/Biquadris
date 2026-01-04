// Player module - interface
export module player;

import <memory>;
import <string>;
import board;
import block;
import level;
import position;
import subject;

/*
 * Player:
 *
 * Abstract base class that represents a player in the game.
 * It stores the core game state shared by all player variants:
 *   - the player's Board
 *   - the Level strategy for block generation
 *   - the currently falling Block, the next Block, and an optional held Block
 *   - the current position of the falling block
 *   - score, level number, and internal counters
 *
 * This class exposes all operations that a player can perform during a turn:
 *   movement, rotation, dropping, holding, level changes, and reset.
 *
 * BasicPlayer implements the actual gameplay logic.
 * Decorator classes (BlindEffect / HeavyEffect / ForceEffect) inherit from Player
 * and selectively override behaviour.
 *
 * Because this is an abstract base class:
 *   - most methods are pure virtual and implemented by BasicPlayer,
 *   - decorator-related helpers provide default "no effect" behaviour here.
 */
export class Player : public Subject {
protected:
    std::shared_ptr<Board> board;      // The player's game board
    std::shared_ptr<Level> level;      // Current Level object controlling block generation
    std::shared_ptr<Block> curBlock;   // Block currently falling
    std::shared_ptr<Block> nextBlock;  // Next block to spawn
    std::shared_ptr<Block> heldBlock;  // Stored block (if any)
    Position curPos;                   // Position of the falling block on the board
    int score = 0;                     // Player's score
    int levelNum = 0;                  // Numerical level (0–4), used by scoring and Level objects
    int blockIdCounter = 0;            // Counter used to assign unique IDs to blocks
    bool alive = true;                 // Player is "alive" unless spawn fails
    bool canHold = true;               // Allow hold only once per turn
    
public:
    Player();
    virtual ~Player();
    
    // ===== Basic getters =====
    // These functions allow observers (text/graphics) and game logic
    // to query the current Player state. Implemented in BasicPlayer or decorators.
    virtual std::shared_ptr<Board> getBoard() = 0;
    virtual int getScore() const = 0;
    virtual int getLevel() const = 0;
    virtual bool isAlive() const = 0;
    virtual std::shared_ptr<Block> getCurBlock() = 0;
    virtual std::shared_ptr<Block> getNextBlock() = 0;
    virtual std::shared_ptr<Block> getHeldBlock() = 0;
    virtual Position getCurPos() const = 0;

    // ===== Block operations =====
    // These define all possible actions during a turn.
    virtual void generateNextBlock() = 0;
    virtual bool spawnBlock() = 0;
    virtual bool move(const std::string& dir) = 0;
    virtual void rotate(const std::string& dir) = 0;
    virtual void drop() = 0;
    virtual void hold() = 0;
    
    // ===== Level adjustments =====
    virtual void levelUp() = 0;
    virtual void levelDown() = 0;
    
    // ===== Reset / configuration =====
    virtual void reset() = 0;                       // Reset player state for a new game
    virtual void setScriptFile(const std::string& filename) = 0;  // For Level 0
    
    // ===== Utility helpers =====
    virtual bool hasCurrentBlock() const = 0;       // True if current block exists
    virtual bool canMoveDown() const = 0;           // Used for drop/auto-drop logic

    // ===== Decorator support =====
    // Default behaviour: no effect applied.
    // Decorator classes override relevant methods while delegating others.
    virtual std::shared_ptr<Player> getWrappedPlayer() const; 
    virtual bool hasBlindEffect() const;
    virtual bool hasHeavyEffect() const;
    virtual bool hasForceEffect() const;
    virtual char getForcedBlockType() const;

    // ===== Effect helper methods =====
    // Decorators override these to access state needed for forced-block logic.
    virtual std::shared_ptr<Level> getLevelObject() const;
    virtual int getNextBlockId() const;      
    
    // Default setNextBlock does nothing; ForceEffect overrides it.
    virtual void setNextBlock(std::shared_ptr<Block> block);
    
    // Replace current block with a new block of specified type
    // Used by ForceEffect to immediately replace opponent's current block
    // Returns true if replacement was successful, false if block cannot be placed (player loses)
    virtual bool replaceCurrentBlock(char blockType);
};
