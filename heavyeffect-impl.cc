// HeavyEffect module - implementation
module heavyeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;

HeavyEffect::HeavyEffect(std::shared_ptr<Player> player) : PlayerEffect{player} {
    // Heavy effect is applied in move() and rotate() methods
}

bool HeavyEffect::move(const std::string& dir) {
    // First do the normal move
    bool moved = wrappedPlayer->move(dir);
    
    if (moved && dir != "down") {
        // After left/right move, automatically drop 2 rows
        // If can't drop 2 rows, drop as much as possible and turn ends
        for (int i = 0; i < 2; ++i) {
            if (!wrappedPlayer->move("down")) {
                // Can't drop further - this ends the turn
                // The block is considered dropped
                break;
            }
        }
    }
    
    return moved;
}

void HeavyEffect::rotate(const std::string& dir) {
    // First do the normal rotation
    wrappedPlayer->rotate(dir);
    
    // After rotation, automatically drop 2 rows
    // If can't drop 2 rows, drop as much as possible
    for (int i = 0; i < 2; ++i) {
        if (!wrappedPlayer->move("down")) {
            // Can't drop further
            break;
        }
    }
}

bool HeavyEffect::hasHeavyEffect() const {
    return true;
}
