// BlindEffect module - interface
export module blindeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;

/*
 * BlindEffect:
 * A decorator that applies the “blind” effect to a player.
 *
 * When applied, the player’s board will hide the central region (rows 3–12,
 * columns 3–9) by displaying '?' instead of the actual contents.  
 * This effect lasts until the player performs their next drop.
 *
 * BlindEffect wraps an existing Player object and overrides drop()
 * so it can remove the blind effect right after the next block is dropped.
 *
 * Only drop() is overridden because blind does not affect movement,
 * rotation, or any other game logic.
 */
export class BlindEffect : public PlayerEffect {
public:
    // Immediately activates the blind effect on the wrapped player's board.
    BlindEffect(std::shared_ptr<Player> player);
    ~BlindEffect() = default;

    // Activates the blind effect at the start of a new turn.
    bool spawnBlock() override;

    // Removes the blind effect right after the wrapped player performs a drop.
    void drop() override;

    // Indicates that this player currently has a blind effect applied.
    bool hasBlindEffect() const override;
};
