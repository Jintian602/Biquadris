// ForceEffect module - implementation
module forceeffect;

import <memory>;
import <string>;
import playereffect;
import player;
import board;
import block;
import level;
import position;
import iblock;
import jblock;
import lblock;
import oblock;
import sblock;
import tblock;
import zblock;

/*
 * ForceEffect:
 * A decorator that forces the player's *current block* to be a specific type.
 *
 * The effect is applied immediately when created:
 * - If the player has a current block, it is replaced immediately.
 * - If the player doesn't have a current block, the next spawned block will be forced.
 *
 * The effect is single-use: once applied, the effect marks itself as "used".
 */
ForceEffect::ForceEffect(std::shared_ptr<Player> player, char blockType)
    : PlayerEffect{player}, forcedBlockType{blockType}, used{false} {
    // Immediately replace current block if it exists
    if (wrappedPlayer->hasCurrentBlock()) {
        if (wrappedPlayer->replaceCurrentBlock(blockType)) {
            used = true; // Effect consumed
        } else {
            // Block cannot be placed - player loses (handled in replaceCurrentBlock)
            used = true;
        }
    }
}

/* The force effect is active only before it has been used once. */
bool ForceEffect::hasForceEffect() const {
    return !used;
}

char ForceEffect::getForcedBlockType() const {
    return forcedBlockType;
}

/*
 * spawnBlock():
 * If the effect hasn’t been used yet, compute the correct block ID + level,
 * construct the forced block manually, and replace the wrapped player’s nextBlock.
 * After that, call wrappedPlayer->spawnBlock() normally.
 */
bool ForceEffect::spawnBlock() {
    // Only force one block
    if (!used) {
        int levelNum = wrappedPlayer->getLevel();
        int blockId  = wrappedPlayer->getNextBlockId();

        std::shared_ptr<Block> forcedBlock;

        // Replace switch-case with else-if chain (your request)
        if (forcedBlockType == 'I') {
            forcedBlock = std::make_shared<IBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'J') {
            forcedBlock = std::make_shared<JBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'L') {
            forcedBlock = std::make_shared<LBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'O') {
            forcedBlock = std::make_shared<OBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'S') {
            forcedBlock = std::make_shared<SBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'T') {
            forcedBlock = std::make_shared<TBlock>(blockId, levelNum);
        } else if (forcedBlockType == 'Z') {
            forcedBlock = std::make_shared<ZBlock>(blockId, levelNum);
        } else {
            // Default fallback — shouldn't happen, but keeps behavior safe
            forcedBlock = std::make_shared<IBlock>(blockId, levelNum);
        }

        // Inject the forced block into the wrapped player
        wrappedPlayer->setNextBlock(forcedBlock);

        used = true; // Effect consumed
    }

    // Delegate to wrapped player's spawnBlock()
    return wrappedPlayer->spawnBlock();
}


