// BasicPlayer module - implementation
module basicplayer;

import <memory>;
import <string>;
import player;
import board;
import block;
import iblock;
import jblock;
import lblock;
import oblock;
import sblock;
import tblock;
import zblock;
import level;
import level0;
import level1;
import level2;
import level3;
import level4;
import position;
import subject;

BasicPlayer::BasicPlayer() : Player{} {
    board = std::make_shared<Board>();
    level = std::make_shared<Level0>();
    lastRowsCleared = 0;
    // Don't generate next block yet - will be generated after script files are set
}

std::shared_ptr<Board> BasicPlayer::getBoard() { return board; }
int BasicPlayer::getScore() const { return score; }
int BasicPlayer::getLevel() const { return levelNum; }
bool BasicPlayer::isAlive() const { return alive; }
std::shared_ptr<Block> BasicPlayer::getCurBlock() { return curBlock; }
std::shared_ptr<Block> BasicPlayer::getNextBlock() { return nextBlock; }
std::shared_ptr<Block> BasicPlayer::getHeldBlock() { return heldBlock; }
Position BasicPlayer::getCurPos() const { return curPos; }

void BasicPlayer::generateNextBlock() {
    nextBlock = level->generateBlock(++blockIdCounter);
    notifyObservers();  // Notify observers when next block is generated
}

bool BasicPlayer::spawnBlock() {
    curBlock = std::move(nextBlock);
    generateNextBlock();
    
    // Reset lock delay state for new block
    isLocked = false;
    lockDelayMoveUsed = false;
    
    // Spawn below the 3 reserve rows (row 3), column 0 (top-left)
    curPos = Position{3, 0};
    
    if (!board->canPlace(*curBlock, curPos)) {
        alive = false;
        notifyObservers();  // Notify observers of state change
        return false;
    }
    
    notifyObservers();  // Notify observers when block is spawned
    return true;
}

bool BasicPlayer::move(const std::string& dir) {
    Position newPos = curPos;
    
    if (dir == "left") newPos.col--;
    else if (dir == "right") newPos.col++;
    else if (dir == "down") newPos.row++;
    
    if (board->canPlace(*curBlock, newPos)) {
        curPos = newPos;
        notifyObservers();  // Notify observers when block moves
        
        // Level 3 and 4 heavy blocks: after horizontal move (left/right), automatically drop 1 row
        if ((levelNum == 3 || levelNum == 4) && (dir == "left" || dir == "right")) {
            Position heavyPos = curPos;
            heavyPos.row++;
            if (board->canPlace(*curBlock, heavyPos)) {
                curPos = heavyPos;
                notifyObservers();
            }
        }
        
        // Lock delay: if block was locked and player moved horizontally, check if still locked
        if (isLocked && (dir == "left" || dir == "right")) {
            setUsedLockDelayMove(true);
            // Check if block can still move down after horizontal move
            if (canMoveDown()) {
                // Block is no longer locked, reset lock state
                setLocked(false);
                setUsedLockDelayMove(false);
            }
        }
        
        return true;
    }
    
    return false;
}

void BasicPlayer::rotate(const std::string& dir) {
    // Get bounding box before rotation
    int oldMinRow, oldMaxRow, oldMinCol, oldMaxCol;
    curBlock->getBoundingBox(oldMinRow, oldMaxRow, oldMinCol, oldMaxCol);
    
    // Perform rotation
    if (dir == "cw") curBlock->rotateCW();
    else curBlock->rotateCCW();
    
    // Get bounding box after rotation
    int newMinRow, newMaxRow, newMinCol, newMaxCol;
    curBlock->getBoundingBox(newMinRow, newMaxRow, newMinCol, newMaxCol);
    
    // Adjust position to preserve lower-left corner of bounding box
    curPos.row += (oldMaxRow - newMaxRow);
    curPos.col += (oldMinCol - newMinCol);
    
    // Check if rotation is valid
    if (!board->canPlace(*curBlock, curPos)) {
        // Revert rotation
        if (dir == "cw") curBlock->rotateCCW();
        else curBlock->rotateCW();
        
        // Restore position
        curPos.row -= (oldMaxRow - newMaxRow);
        curPos.col -= (oldMinCol - newMinCol);
    } else {
        notifyObservers();  // Notify observers when block rotates
        
        // Level 3 and 4 heavy blocks: after rotation, automatically drop 1 row
        if (levelNum == 3 || levelNum == 4) {
            Position heavyPos = curPos;
            heavyPos.row++;
            if (board->canPlace(*curBlock, heavyPos)) {
                curPos = heavyPos;
                notifyObservers();
            }
        }
        
        // Lock delay: if block was locked and player rotated, check if still locked
        if (isLocked) {
            setUsedLockDelayMove(true);
            // Check if block can still move down after rotation
            if (canMoveDown()) {
                // Block is no longer locked, reset lock state
                setLocked(false);
                setUsedLockDelayMove(false);
            }
        }
    }
}

void BasicPlayer::drop() {
    while (move("down")) {}
    
    board->place(*curBlock, curPos);
    auto result = board->clearFullRowsWithBlockInfo();
    lastRowsCleared = result.rowsCleared;
    
    // Level 4: External constructive force
    // Every 5 blocks dropped without clearing at least one row, drop a 1x1 '*' block in center column
    if (levelNum == 4) {
        if (result.rowsCleared == 0) {
            blocksDroppedWithoutClear++;
            // Drop 1x1 '*' block in center column
            if (blocksDroppedWithoutClear % 5 == 0) {
                int centerCol = board->getCols() / 2;  // Center column (11 columns: 0-10, so 5 is center)
                board->drop(centerCol);
                notifyObservers();
            }
        } else {
            // Row was cleared, reset counter
            blocksDroppedWithoutClear = 0;
        }
    }
    
    // Line Clearing scoring: (current level + number of lines cleared)^2
    if (result.rowsCleared > 0) {
        int lineClearingScore = (levelNum + result.rowsCleared) * (levelNum + result.rowsCleared);
        score += lineClearingScore;
    }
    
    // Block Removal scoring: (born level + 1)^2 for each completely removed block
    for (int bornLevel : result.removedBlockBornLevels) {
        int blockRemovalScore = (bornLevel + 1) * (bornLevel + 1);
        score += blockRemovalScore;
    }
    
    canHold = true;  // Reset hold ability after drop
    
    // Clear current block - Game will spawn for the next player
    curBlock = nullptr;
    
    notifyObservers();  // Notify observers when block is dropped
}

void BasicPlayer::hold() {
    // Can only hold if in top row (below reserve rows = row 3) and haven't held yet this turn
    if (curPos.row != 3 || !canHold) return;
    
    if (!heldBlock) {
        // First time holding - store current and spawn next
        heldBlock = curBlock;
        spawnBlock();
    } else {
        // Swap current with held
        std::shared_ptr<Block> temp = curBlock;
        curBlock = heldBlock;
        heldBlock = temp;
        curPos = Position{3, 0};
        
        // Check if swapped block can be placed
        if (!board->canPlace(*curBlock, curPos)) {
            // Swap back if can't place
            curBlock = heldBlock;
            heldBlock = temp;
            return;
        }
    }
    
    canHold = false;  // Can only hold once per turn
    notifyObservers();  // Notify observers when block is held
}

void BasicPlayer::levelUp() {
    if (levelNum < 4) {
        ++levelNum;
        if (levelNum == 0) {
            level = std::make_shared<Level0>();
        } else if (levelNum == 1) {
            level = std::make_shared<Level1>();
        } else if (levelNum == 2) {
            level = std::make_shared<Level2>();
        } else if (levelNum == 3) {
            level = std::make_shared<Level3>();
        } else if (levelNum == 4) {
            level = std::make_shared<Level4>();
        }
        // Regenerate next block with new level
        if (!curBlock) {
            // If no current block spawned yet, regenerate next block
            generateNextBlock();
        }
        notifyObservers();  // Notify observers when level changes
    }
}

void BasicPlayer::levelDown() {
    if (levelNum > 0) {
        --levelNum;
        if (levelNum == 0) {
            level = std::make_shared<Level0>();
            if (auto lvl0 = std::dynamic_pointer_cast<Level0>(level)) {
                lvl0->setScriptFile(level0ScriptFile);
            }
        } else if (levelNum == 1) {
            level = std::make_shared<Level1>();
        } else if (levelNum == 2) {
            level = std::make_shared<Level2>();
        } else if (levelNum == 3) {
            level = std::make_shared<Level3>();
        }
        // Regenerate next block with new level
        if (!curBlock) {
            generateNextBlock();
        }
        notifyObservers();  // Notify observers when level changes
    }
}

void BasicPlayer::reset() {
    board->reset();
    score = 0;
    levelNum = 0;
    // Create a new Level0 based on the currently saved script file.
    level = std::make_shared<Level0>();
    if (auto lvl0 = std::dynamic_pointer_cast<Level0>(level)) {
        lvl0->setScriptFile(level0ScriptFile);
    }
    blockIdCounter = 0;
    alive = true;
    curBlock = nullptr;
    heldBlock = nullptr;
    canHold = true;
    lastRowsCleared = 0;
    generateNextBlock();
    // Don't spawn block yet - wait for player's turn
}

void BasicPlayer::setScriptFile(const std::string& filename) {
    level0ScriptFile = filename;
    if (levelNum == 0) {
        if (auto lvl0 = std::dynamic_pointer_cast<Level0>(level)) {
            lvl0->setScriptFile(level0ScriptFile);
        }
    }
}

bool BasicPlayer::hasCurrentBlock() const {
    return curBlock != nullptr;
}

bool BasicPlayer::canMoveDown() const {
    if (!curBlock) return false;
    Position downPos = curPos;
    downPos.row++;
    return board->canPlace(*curBlock, downPos);
}

int BasicPlayer::getRowsCleared() const {
    return lastRowsCleared;
}

bool BasicPlayer::canApplySpecial() const {
    return lastRowsCleared >= 2;
}

bool BasicPlayer::isBlockLocked() const {
    return isLocked;
}

bool BasicPlayer::hasUsedLockDelayMove() const {
    return lockDelayMoveUsed;
}

void BasicPlayer::setLocked(bool locked) {
    isLocked = locked;
}

void BasicPlayer::setUsedLockDelayMove(bool used) {
    lockDelayMoveUsed = used;
}

std::shared_ptr<Level> BasicPlayer::getLevelObject() const {
    return level;
}

int BasicPlayer::getNextBlockId() const {
    return blockIdCounter + 1;  // Next block ID that will be used
}

void BasicPlayer::setNextBlock(std::shared_ptr<Block> block) {
    nextBlock = block;
    notifyObservers();  // Notify observers when next block changes
}

bool BasicPlayer::replaceCurrentBlock(char blockType) {
    // Only replace if there's a current block
    if (!curBlock) {
        return false;
    }
    
    // Create the new block with the same ID and level
    int blockId = curBlock->getId();
    int levelNum = getLevel();
    std::shared_ptr<Block> newBlock;
    
    if (blockType == 'I') {
        newBlock = std::make_shared<IBlock>(blockId, levelNum);
    } else if (blockType == 'J') {
        newBlock = std::make_shared<JBlock>(blockId, levelNum);
    } else if (blockType == 'L') {
        newBlock = std::make_shared<LBlock>(blockId, levelNum);
    } else if (blockType == 'O') {
        newBlock = std::make_shared<OBlock>(blockId, levelNum);
    } else if (blockType == 'S') {
        newBlock = std::make_shared<SBlock>(blockId, levelNum);
    } else if (blockType == 'T') {
        newBlock = std::make_shared<TBlock>(blockId, levelNum);
    } else if (blockType == 'Z') {
        newBlock = std::make_shared<ZBlock>(blockId, levelNum);
    } else {
        // Invalid block type
        return false;
    }
    
    // Place new block at initial position (row 3, col 0)
    Position initialPos{3, 0};
    
    // Check if the new block can be placed at initial position
    if (!board->canPlace(*newBlock, initialPos)) {
        // Cannot place - player loses
        alive = false;
        curBlock = nullptr;
        notifyObservers();
        return false;
    }
    
    // Replace current block
    curBlock = newBlock;
    curPos = initialPos;
    notifyObservers();
    return true;
}

BasicPlayer::~BasicPlayer() {}

