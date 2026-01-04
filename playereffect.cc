// PlayerEffect module - interface (Abstract Decorator)
export module playereffect;

import <memory>;
import <string>;
import player;
import board;
import block;
import level;
import position;

/*
 * PlayerEffect (Abstract Decorator)
 * ---------------------------------
 * This class implements the Decorator pattern used in the game.
 *
 * Purpose:
 *   - To allow dynamic addition of temporary player effects (Blind, Heavy, Force).
 *   - To wrap an existing Player object while preserving the Player interface.
 *   - To intercept or modify behaviour of specific Player methods without
 *     modifying BasicPlayer directly.
 *
 * Responsibilities:
 *   - Store a wrapped Player (either BasicPlayer or another PlayerEffect).
 *   - By default, forward every operation to the wrapped Player.
 *   - Concrete decorators override only the methods they affect.
 *
 * Notes for Marker:
 *   - This abstraction ensures OCP compliance (open for extension, closed for modification).
 *   - Observers attach to the outermost decorator, so rendering always reflects the effect.
 */
export class PlayerEffect : public Player {
protected:
    // The underlying Player being decorated.
    // This may be a BasicPlayer or another PlayerEffect (chained decorators).
    std::shared_ptr<Player> wrappedPlayer;

public:
    // Construct a decorator that wraps the given Player.
    explicit PlayerEffect(std::shared_ptr<Player> player);
    virtual ~PlayerEffect();

    // ===== Basic getters (default: forward to wrappedPlayer) =====
    std::shared_ptr<Board> getBoard() override;
    int getScore() const override;
    int getLevel() const override;
    bool isAlive() const override;
    std::shared_ptr<Block> getCurBlock() override;
    std::shared_ptr<Block> getNextBlock() override;
    std::shared_ptr<Block> getHeldBlock() override;
    Position getCurPos() const override;

    // ===== Block movement operations =====
    // These are the core runtime actions; decorators override to add behaviour.
    void generateNextBlock() override;
    bool spawnBlock() override;
    bool move(const std::string& dir) override;
    void rotate(const std::string& dir) override;
    void drop() override;
    void hold() override;

    // ===== Level operations =====
    void levelUp() override;
    void levelDown() override;

    // ===== Game state operations =====
    void reset() override;
    void setScriptFile(const std::string& filename) override;

    // ===== Utility checks =====
    bool hasCurrentBlock() const override;
    bool canMoveDown() const override;

    // ===== Effect helper operations =====
    // Used internally by ForceEffect / HeavyEffect / BlindEffect.
    std::shared_ptr<Level> getLevelObject() const override;
    int getNextBlockId() const override;
    void setNextBlock(std::shared_ptr<Block> block) override;
    bool replaceCurrentBlock(char blockType) override;

    // Returns the Player underneath this decorator.
    // Used to unwrap the decorator chain.
    std::shared_ptr<Player> getWrappedPlayer() const override;
};
