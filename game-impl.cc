/**
 * @file game-impl.cc
 * @brief Implementation of the Game class
 * 
 * This file contains the implementation of all Game methods, including
 * initialization, command handling, and game flow control.
 * Note: Display is NOT handled here - it's done via Observer pattern.
 */

module game;

import <memory>;
import <iostream>;
import <string>;
import <sstream>;
import player;
import basicplayer;
import blindeffect;
import heavyeffect;
import forceeffect;

Game::Game() {
    p1 = std::make_shared<BasicPlayer>();
    p2 = std::make_shared<BasicPlayer>();
}

void Game::run() {
    // Spawn block for the first player (Player 1)
    p1->spawnBlock();
    // Player 2 will spawn when it's their turn
}

std::shared_ptr<Player> Game::getPlayer1() {
    return p1;
}

std::shared_ptr<Player> Game::getPlayer2() {
    return p2;
}

std::shared_ptr<Player> Game::getCurrentPlayer() {
    return current == 1 ? p1 : p2;
}

void Game::switchTurn() {
    current = (current == 1) ? 2 : 1;
}

bool Game::handleCommand(const std::string& cmd, int multiplier, int* droppingPlayerNum, bool* blockDropped) {
    auto player = getCurrentPlayer();
    bool shouldApplySpecial = false;
    bool dropped = false;
    
    // Execute the command 'multiplier' times
    for (int i = 0; i < multiplier; ++i) {
        // Movement commands
        if (cmd == "left") {
            player->move("left");
        } else if (cmd == "right") {
            player->move("right");
        } else if (cmd == "down") {
            player->move("down");
        } 
        // Rotation commands
        else if (cmd == "cw" || cmd == "clockwise") {
            player->rotate("cw");
        } else if (cmd == "ccw" || cmd == "counterclockwise") {
            player->rotate("ccw");
        } 
        // Drop command
        else if (cmd == "drop") {
            // Store current player number BEFORE drop to know who cleared rows
            int droppingPlayer = current;
            
            player->drop();
            dropped = true;
            
            // Check if special effect should be applied
            // Unwrap decorators to get BasicPlayer
            std::shared_ptr<Player> unwrapped = player;
            while (unwrapped->getWrappedPlayer()) {
                unwrapped = unwrapped->getWrappedPlayer();
            }
            if (auto basic = std::dynamic_pointer_cast<BasicPlayer>(unwrapped)) {
                if (basic->canApplySpecial()) {
                    shouldApplySpecial = true;
                    // Store which player dropped
                    // This will be used to determine the opponent
                    if (droppingPlayerNum) {
                        *droppingPlayerNum = droppingPlayer;
                    }
                }
            }
        } 
        // Hold command
        else if (cmd == "hold") {
            player->hold();
            break; // Only execute once
        } 
        // Level adjustment commands (applied once regardless of multiplier)
        else if (cmd == "levelup") {
            player->levelUp();
            break; // Only execute once
        } else if (cmd == "leveldown") {
            player->levelDown();
            break; // Only execute once
        }
        // Testing commands: replace current block with specific type
        else if (cmd == "I" || cmd == "J" || cmd == "L" || cmd == "O" || 
                 cmd == "S" || cmd == "T" || cmd == "Z") {
            // TODO: Implement block replacement (handled by Player/Level)
            // player->replaceCurrentBlock(cmd[0]);
            break; // Only execute once for block replacement
        }
        // Special commands
        else if (cmd == "hint" || cmd == "norandom" || cmd == "random" || cmd == "sequence") {
            // TODO: Implement these special commands
            break; // These don't use multipliers
        }
        
        // Per Piazza clarification: blocks can move/rotate as much as you like on the bottom,
        // even if it causes collision, until drop command is issued.
        // Removed lock delay auto-drop logic to allow unlimited moves at bottom.
    }
    
    // Set output parameter if block was dropped
    if (blockDropped) {
        *blockDropped = dropped;
    }
    
    // No need to call updateDisplay() - Board automatically notifies observers
    // when its state changes (Observer pattern)
    
    return shouldApplySpecial;
}

bool Game::isGameOver() {
    return !p1->isAlive() || !p2->isAlive();
}

void Game::restart() {
    // Unwrap any effects before reset
    while (p1->getWrappedPlayer()) {
        p1 = p1->getWrappedPlayer();
    }
    while (p2->getWrappedPlayer()) {
        p2 = p2->getWrappedPlayer();
    }
    
    p1->reset();
    p2->reset();
    current = 1;
    // Spawn block for Player 1 (first player)
    p1->spawnBlock();
    // Display updates happen automatically through Observer pattern
}

bool Game::canApplySpecial() const {
    // Unwrap decorators to get BasicPlayer
    std::shared_ptr<Player> unwrapped = (current == 1) ? p1 : p2;
    while (unwrapped->getWrappedPlayer()) {
        unwrapped = unwrapped->getWrappedPlayer();
    }
    if (auto basic = std::dynamic_pointer_cast<BasicPlayer>(unwrapped)) {
        return basic->canApplySpecial();
    }
    return false;
}

bool Game::applySpecialEffect(const std::string& effect, int targetPlayer) {
    // Determine which player to apply effect to
    // If targetPlayer is 0, apply to opponent of current player
    // Otherwise, apply to specified player
    std::shared_ptr<Player> opponent;
    if (targetPlayer == 0) {
        // Apply to opponent of current player
        opponent = (current == 1) ? p2 : p1;
    } else if (targetPlayer == 1) {
        opponent = p1;
    } else if (targetPlayer == 2) {
        opponent = p2;
    } else {
        std::cout << "Invalid target player. Must be 1 or 2." << std::endl;
        return false;
    }
    
    // Get observers before applying effect (to re-attach after)
    auto observers = opponent->getObservers();
    
    // Parse effect command
    std::istringstream iss(effect);
    std::string effectType;
    iss >> effectType;
    
    std::shared_ptr<Player> newOpponent;
    
    // Apply new effect by wrapping the existing opponent (which may already have effects)
    // This allows effects to accumulate cumulatively
    if (effectType == "blind") {
        // Apply BlindEffect by wrapping existing opponent
        newOpponent = std::make_shared<BlindEffect>(opponent);
    } else if (effectType == "heavy") {
        // Apply HeavyEffect by wrapping existing opponent
        newOpponent = std::make_shared<HeavyEffect>(opponent);
    } else if (effectType == "force") {
        // Get block type
        std::string blockTypeStr;
        iss >> blockTypeStr;
        if (!blockTypeStr.empty()) {
            char blockType = blockTypeStr[0];
            // Validate block type
            if (blockType != 'I' && blockType != 'J' && blockType != 'L' && 
                blockType != 'O' && blockType != 'S' && blockType != 'T' && blockType != 'Z') {
                std::cout << "Invalid block type: '" << blockType << "'. Valid types are: I, J, L, O, S, T, Z" << std::endl;
                return false;
            }
            // Apply ForceEffect by wrapping existing opponent
            newOpponent = std::make_shared<ForceEffect>(opponent, blockType);
        } else {
            std::cout << "Invalid force command. Please specify a block type: force <blockType>" << std::endl;
            std::cout << "Valid block types are: I, J, L, O, S, T, Z" << std::endl;
            std::cout << "Example: force Z" << std::endl;
            return false;
        }
    } else {
        std::cout << "Invalid effect type: '" << effectType << "'" << std::endl;
        std::cout << "Valid effects are:" << std::endl;
        std::cout << "  blind - Blind opponent's board (columns 3-9, rows 3-12)" << std::endl;
        std::cout << "  heavy - Make opponent's blocks heavy (auto-drop 2 rows after move/rotate)" << std::endl;
        std::cout << "  force <blockType> - Force opponent's next block type (e.g., force Z)" << std::endl;
        std::cout << "Valid block types for force: I, J, L, O, S, T, Z" << std::endl;
        return false;
    }
    
    // Replace target player with decorated version
    if (targetPlayer == 0) {
        // Use current to determine opponent
        if (current == 1) {
            p2 = newOpponent;
        } else {
            p1 = newOpponent;
        }
    } else if (targetPlayer == 1) {
        p1 = newOpponent;
    } else if (targetPlayer == 2) {
        p2 = newOpponent;
    }
    
    // Re-attach observers to the new decorated player
    for (auto obs : observers) {
        newOpponent->attach(obs);
    }
    
    // Note: canApplySpecial flag is reset in BasicPlayer::drop() 
    // when lastRowsCleared is set for the next drop
    return true;
}


