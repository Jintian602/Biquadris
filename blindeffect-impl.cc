// BlindEffect module - implementation
module blindeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;

BlindEffect::BlindEffect(std::shared_ptr<Player> player) : PlayerEffect{player} {
    // When first applied, spawnBlock() has already been called for the current turn
    // So we activate blind immediately for this ongoing turn
    if (auto board = getBoard()) {
        board->setBlind(true);  // Enable blind effect immediately (first application)
        notifyObservers();  // Notify observers to update display
    }
}

bool BlindEffect::spawnBlock() {
    // This is called at the START of a new turn
    // Blind effect should be active during the turn, so activate it BEFORE spawnBlock
    if (auto board = getBoard()) {
        board->setBlind(true);  // Enable blind effect at turn start
    }
    // Call wrapped player's spawnBlock (this will notify observers with blind ON)
    bool result = wrappedPlayer->spawnBlock();
    return result;
}

void BlindEffect::drop() {
    // Disable blind effect BEFORE drop (turn is ending)
    // This ensures that when wrappedPlayer->drop() calls notifyObservers(),
    // observers see the blind effect as OFF
    if (auto board = getBoard()) {
        board->setBlind(false);  // Disable blind effect before drop completes
    }
    // Call wrapped player's drop (this will notify observers with blind OFF)
    wrappedPlayer->drop();
    // Note: wrappedPlayer->drop() already calls notifyObservers() with blind=false
}

bool BlindEffect::hasBlindEffect() const {
    return true;
}
