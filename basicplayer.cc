// BasicPlayer module - interface
export module basicplayer;

import <memory>;
import <string>;
import player;
import board;
import block;
import level;
import position;

/*
 * BasicPlayer
 *
 * Represents a single player in the Biquadris game.
 * Manages:
 *   - the board
 *   - current / next / held blocks
 *   - player score and level
 *   - movement, rotation, dropping, holding
 *   - level changes and scriptfile handling (for Level 0)
 *
 * Effects (blind / heavy / force) are handled through decorators,
 * so BasicPlayer only contains base game logic.
 */
export class BasicPlayer : public Player {
public:
    BasicPlayer();
    ~BasicPlayer();
    
    // === Getters ===
    std::shared_ptr<Board> getBoard() override;
    int getScore() const override;
    int getLevel() const override;          // current level number
    bool isAlive() const override;
    std::shared_ptr<Block> getCurBlock() override;
    std::shared_ptr<Block> getNextBlock() override;
    std::shared_ptr<Block> getHeldBlock() override;
    Position getCurPos() const override;    // current origin of the falling block
    
    // === Block operations ===
    void generateNextBlock() override;      // asks level to generate a new nextBlock
    bool spawnBlock() override;             // moves nextBlock → curBlock; returns false if game over
    bool move(const std::string& dir) override;   // left / right / down
    void rotate(const std::string& dir) override; // cw / ccw
    void drop() override;                   // hard drop; updates scoring
    
    // Hold mechanic (only allowed once per turn)
    void hold() override;

    // === Level operations ===
    void levelUp() override;
    void levelDown() override;

    // === Game operations ===
    void reset() override;                  // full game reset
    void setScriptFile(const std::string& filename) override; // for Level 0 only
    
    // === Utility ===
    bool hasCurrentBlock() const override;  // true if curBlock != nullptr
    bool canMoveDown() const override;      // checks if current block can drop 1 row
    
    // Information for applying special effects
    int getRowsCleared() const;             // last # rows cleared by drop()
    bool canApplySpecial() const;           // true if cleared ≥ 2 rows
    
    // Lock delay state (for formal Tetris behavior)
    bool isBlockLocked() const;             // true if block has touched ground and is in lock delay
    bool hasUsedLockDelayMove() const;      // true if player has used the one allowed move after touching ground
    void setLocked(bool locked);            // set lock state (used internally)
    void setUsedLockDelayMove(bool used);   // set lock delay move state (used internally)
    
    // Helpers used by decorator effects
    std::shared_ptr<Level> getLevelObject() const override;
    int getNextBlockId() const override;
    void setNextBlock(std::shared_ptr<Block> block) override; // used by ForceEffect
    bool replaceCurrentBlock(char blockType) override; // used by ForceEffect
    
private:
    int lastRowsCleared = 0;                // tracks how many rows were cleared last drop
    int blocksDroppedWithoutClear = 0;      // Level 4: tracks blocks dropped without clearing rows
    bool isLocked = false;                  // true if block has touched ground and is waiting for lock delay
    bool lockDelayMoveUsed = false;         // true if player has used the one allowed move after touching ground
    std::string level0ScriptFile = "sequence.txt";  
    // Stores the currently active script file for Level0.
    // When scriptfile is changed during the game, this keeps the new path so that:
    //   - Level0 uses the latest script file
    //   - When player resets or levels down from L1+ → L0, the same script is used
};
