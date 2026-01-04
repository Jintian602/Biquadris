// ForceEffect module - interface
export module forceeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;

/*
 * ForceEffect:
 * ---------------
 * A Player decorator that forces the next block the player receives to be a
 * specific block type (I, J, L, O, S, T, or Z).
 *
 * - This effect is *single-use*: it only affects one spawn.
 * - It overrides spawnBlock(), injects a replacement nextBlock, and then
 *   delegates the spawn to the wrapped player.
 * - After one forced spawn, the decorator marks itself as "used".
 *
 * The Game module will attach this effect to a player whenever another player
 * clears 2+ lines (special action). Observers remain attached to the outermost
 * decorator, so the UI continues to receive updates normally.
 */
export class ForceEffect : public PlayerEffect {
    char forcedBlockType;   // Block type being forced (I, J, L, O, S, T, Z)
    bool used = false;      // Whether the effect has already been applied

public:
    /*
     * Construct a force effect applied on top of another Player.
     * `blockType` must be one of the seven standard block types.
     */
    ForceEffect(std::shared_ptr<Player> player, char blockType);
    ~ForceEffect() = default;

    /*
     * spawnBlock():
     * -------------
     * If the effect has not yet been used, this replaces the player's
     * nextBlock with the forced block (using the same ID + level),
     * then forwards to the wrapped player's spawnBlock().
     *
     * After the first forced spawn, the effect becomes inactive.
     */
    bool spawnBlock() override;

    // Used by Game/UI to check whether the effect is still active
    bool hasForceEffect() const override;

    // Returns the block type this effect will force upon the next spawn
    char getForcedBlockType() const override;
};
