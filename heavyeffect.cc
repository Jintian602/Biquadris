// HeavyEffect module - interface
export module heavyeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;

/*
 * HeavyEffect:
 * ------------
 * A Player decorator that makes the controlled block “heavier”.
 *
 * Behaviour:
 *   - After every left/right move, the block automatically moves down 2 rows.
 *   - After every rotation, the block automatically moves down 2 rows.
 *
 * If the block cannot drop the full 2 rows, it drops as far as possible.
 * The decorator does NOT override drop(): drop behaviour remains unchanged.
 *
 * This effect lasts until the player's turn ends (i.e., the block lands and
 * the next spawn happens). Game attaches this effect when an opponent clears
 * 2+ lines.
 *
 * Observers remain attached to the outermost decorator, so UI updates
 * continue as usual.
 */
export class HeavyEffect : public PlayerEffect {
public:
    // Wrap an existing player with heavy behaviour
    HeavyEffect(std::shared_ptr<Player> player);
    ~HeavyEffect() = default;
    
    /*
     * move(dir):
     * ----------
     * Performs the normal move via wrappedPlayer->move(),
     * then — if dir is "left" or "right" — applies two automatic downward moves.
     */
    bool move(const std::string& dir) override;
    
    /*
     * rotate(dir):
     * ------------
     * Performs the normal rotation, then applies two downward moves.
     */
    void rotate(const std::string& dir) override;
    
    // Used by Game or UI to detect that this decorator is a heavy effect
    bool hasHeavyEffect() const override;
};
