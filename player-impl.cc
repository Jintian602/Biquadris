module player;

import <memory>;
import <string>;
import board;
import block;
import level;
import position;
import subject;

/*
 * Default implementations for base Player.
 * These are used by decorators or BasicPlayer to fall back.
 * Most functions return neutral values.
 */

Player::Player() {}

Player::~Player() {}

/*
 * For decorator pattern.
 * Base Player has no wrapped player → return nullptr.
 */
std::shared_ptr<Player> Player::getWrappedPlayer() const {
    return nullptr;
}

/*
 * Default effect flags – no effect on base player.
 */
bool Player::hasBlindEffect() const { return false; }
bool Player::hasHeavyEffect() const { return false; }
bool Player::hasForceEffect() const { return false; }

/*
 * Default forced block type – none.
 */
char Player::getForcedBlockType() const { return '\0'; }

/*
 * Level/ID helpers — overridden in BasicPlayer.
 */
std::shared_ptr<Level> Player::getLevelObject() const { return nullptr; }
int Player::getNextBlockId() const { return 0; }

/*
 * Default setNextBlock — only ForceEffect overrides this.
 */
void Player::setNextBlock(std::shared_ptr<Block> block) {
    // Default: do nothing
}

/*
 * Default replaceCurrentBlock — returns false (no effect).
 */
bool Player::replaceCurrentBlock(char blockType) {
    // Default: do nothing, return false
    return false;
}







































