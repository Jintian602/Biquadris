module playereffect;

import <memory>;
import <string>;
import player;
import board;
import block;
import level;
import position;

using std::shared_ptr;
using std::string;

/*
 * Implementation Notes:
 * ---------------------
 * PlayerEffect does not alter any behaviour by itself.
 * Its only role is to forward calls to the wrappedPlayer.
 *
 * Concrete subclasses (e.g., HeavyEffect) override specific methods
 * while relying on PlayerEffect to provide default behaviour for the rest.
 */

PlayerEffect::PlayerEffect(shared_ptr<Player> player)
    : wrappedPlayer{player} {
    // Observers attach to the decorator (Game handles this),
    // so visual effects reflect the modified behaviour.
}

PlayerEffect::~PlayerEffect() {}

// ===== Getter forwarding =====

shared_ptr<Board> PlayerEffect::getBoard() {
    return wrappedPlayer->getBoard();
}

int PlayerEffect::getScore() const {
    return wrappedPlayer->getScore();
}

int PlayerEffect::getLevel() const {
    return wrappedPlayer->getLevel();
}

bool PlayerEffect::isAlive() const {
    return wrappedPlayer->isAlive();
}

shared_ptr<Block> PlayerEffect::getCurBlock() {
    return wrappedPlayer->getCurBlock();
}

shared_ptr<Block> PlayerEffect::getNextBlock() {
    return wrappedPlayer->getNextBlock();
}

shared_ptr<Block> PlayerEffect::getHeldBlock() {
    return wrappedPlayer->getHeldBlock();
}

Position PlayerEffect::getCurPos() const {
    return wrappedPlayer->getCurPos();
}

// ===== Block actions =====

void PlayerEffect::generateNextBlock() {
    wrappedPlayer->generateNextBlock();
}

bool PlayerEffect::spawnBlock() {
    return wrappedPlayer->spawnBlock();
}

bool PlayerEffect::move(const string& dir) {
    return wrappedPlayer->move(dir);
}

void PlayerEffect::rotate(const string& dir) {
    wrappedPlayer->rotate(dir);
}

void PlayerEffect::drop() {
    wrappedPlayer->drop();
}

void PlayerEffect::hold() {
    wrappedPlayer->hold();
}

// ===== Level control =====

void PlayerEffect::levelUp() {
    wrappedPlayer->levelUp();
}

void PlayerEffect::levelDown() {
    wrappedPlayer->levelDown();
}

// ===== Reset and script handling =====

void PlayerEffect::reset() {
    wrappedPlayer->reset();
}

void PlayerEffect::setScriptFile(const string& filename) {
    wrappedPlayer->setScriptFile(filename);
}

// ===== Utility =====

bool PlayerEffect::hasCurrentBlock() const {
    return wrappedPlayer->hasCurrentBlock();
}

bool PlayerEffect::canMoveDown() const {
    return wrappedPlayer->canMoveDown();
}

// ===== Effect helper forwarding =====

shared_ptr<Level> PlayerEffect::getLevelObject() const {
    return wrappedPlayer->getLevelObject();
}

int PlayerEffect::getNextBlockId() const {
    return wrappedPlayer->getNextBlockId();
}

void PlayerEffect::setNextBlock(shared_ptr<Block> block) {
    wrappedPlayer->setNextBlock(block);
}

bool PlayerEffect::replaceCurrentBlock(char blockType) {
    return wrappedPlayer->replaceCurrentBlock(blockType);
}

// ===== Unwrap =====

shared_ptr<Player> PlayerEffect::getWrappedPlayer() const {
    return wrappedPlayer;
}
