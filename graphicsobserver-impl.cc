/**
 * @file graphicsobserver-impl.cc
 * @brief Implementation of the GraphicsObserver class
 * 
 * This file contains the implementation of all GraphicsObserver methods,
 * including rendering logic for the graphical display of the Biquadris game.
 */

module graphicsobserver;

import <memory>;
import <sstream>;
import textobserver;
import player;
import board;
import block;
import position;
import xwindow;

GraphicsObserver::GraphicsObserver(Player* p1, Player* p2) 
    : TextObserver{p1, p2}, showPhantom(true) {
    xw = std::make_shared<XWindow>(660, 850);
}

void GraphicsObserver::notify() {
    draw();
}

int GraphicsObserver::getColorForSymbol(char sym) {
    // Map each block type character to its corresponding color
    switch (sym) {
        case 'I': return XWindow::Cyan;      // I-block: cyan (4x1 line)
        case 'J': return XWindow::Blue;      // J-block: blue
        case 'L': return XWindow::Orange;    // L-block: orange
        case 'O': return XWindow::Yellow;    // O-block: yellow (2x2 square)
        case 'S': return XWindow::Green;     // S-block: green
        case 'T': return XWindow::Magenta;   // T-block: magenta
        case 'Z': return XWindow::Red;       // Z-block: red
        case '*': return XWindow::Brown;     // Special/bonus block: brown
        default: return XWindow::White;      // Unknown: white
    }
}

void GraphicsObserver::draw() {
    // Get all information from Player (Subject)
    auto board1 = player1->getBoard();
    auto board2 = player2->getBoard();
    int level1 = player1->getLevel();
    int level2 = player2->getLevel();
    int score1 = player1->getScore();
    int score2 = player2->getScore();
    
    auto curBlock1 = player1->getCurBlock();
    auto curBlock2 = player2->getCurBlock();
    Position curPos1 = player1->getCurPos();
    Position curPos2 = player2->getCurPos();
    
    auto nextBlock1 = player1->getNextBlock();
    auto nextBlock2 = player2->getNextBlock();
    auto heldBlock1 = player1->getHeldBlock();
    auto heldBlock2 = player2->getHeldBlock();
    
    // Background - medium gray
    xw->fillRectangle(0, 0, 660, 850, XWindow::DarkGray);
    
    // High Score bar at top center
    int highScore = (score1 > score2) ? score1 : score2;
    int highScoreWidth = 290;
    int highScoreX = (660 - highScoreWidth) / 2;
    xw->fillRectangle(highScoreX, 10, highScoreWidth, 35, XWindow::Yellow);
    xw->drawRectangle(highScoreX, 10, highScoreWidth, 35, XWindow::Black);
    std::ostringstream ossHigh;
    ossHigh << "High Score:        " << highScore;
    xw->drawString(highScoreX + 60, 32, ossHigh.str(), XWindow::Black);
    
    // Player 1 column (left)
    int p1X = MARGIN;
    drawInfoBox(p1X, 60, 1, level1, score1);
    drawBoard(board1.get(), p1X, 130, curBlock1.get(), curPos1);
    drawNextBlockBox(p1X, 130 + BOARD_HEIGHT + 10, nextBlock1.get());
    drawHeldBlockBox(p1X, 130 + BOARD_HEIGHT + 120, heldBlock1.get());
    
    // Player 2 column (right)
    int p2X = MARGIN + PLAYER_SPACING;
    drawInfoBox(p2X, 60, 2, level2, score2);
    drawBoard(board2.get(), p2X, 130, curBlock2.get(), curPos2);
    drawNextBlockBox(p2X, 130 + BOARD_HEIGHT + 10, nextBlock2.get());
    drawHeldBlockBox(p2X, 130 + BOARD_HEIGHT + 120, heldBlock2.get());
}

void GraphicsObserver::drawInfoBox(int x, int y, int playerNum, int level, int score) {
    // Draw colored background - softer colors like Tetris
    if (playerNum == 2) {
        // Softer pink/salmon color
        xw->fillRectangle(x, y, INFO_BOX_WIDTH, INFO_BOX_HEIGHT, XWindow::Magenta);
    } else {
        xw->fillRectangle(x, y, INFO_BOX_WIDTH, INFO_BOX_HEIGHT, XWindow::Cyan);
    }
    xw->drawRectangle(x, y, INFO_BOX_WIDTH, INFO_BOX_HEIGHT, XWindow::Black);
    
    // Player label - left aligned
    std::ostringstream oss;
    oss << "Player " << playerNum;
    xw->drawString(x + 10, y + 20, oss.str(), XWindow::Black);
    
    // Level - left side
    std::ostringstream ossL;
    ossL << "Level:        " << level;
    xw->drawString(x + 10, y + 42, ossL.str(), XWindow::Black);
    
    // Score - right side
    std::ostringstream ossS;
    ossS << "Score:        " << score;
    xw->drawString(x + 150, y + 42, ossS.str(), XWindow::Black);
}

void GraphicsObserver::drawNextBlockBox(int x, int y, Block* nextBlock) {
    // Draw box - light gray background with dark border
    xw->fillRectangle(x, y, INFO_BOX_WIDTH, 95, XWindow::LightGray);
    xw->drawRectangle(x, y, INFO_BOX_WIDTH, 95, XWindow::Black);
    xw->drawRectangle(x - 1, y - 1, INFO_BOX_WIDTH + 2, 97, XWindow::Black);
    
    // Label - centered
    xw->drawString(x + 100, y + 20, "Next Block", XWindow::Black);
    
    // Preview area - slightly lighter
    xw->fillRectangle(x + 10, y + 30, INFO_BOX_WIDTH - 20, 55, XWindow::White);
    xw->drawRectangle(x + 10, y + 30, INFO_BOX_WIDTH - 20, 55, XWindow::Black);
    
    // Draw the next block in the preview area
    if (nextBlock) {
        int cellSize = 12;  // Smaller cells for preview
        
        // Calculate block bounds to center it properly
        int minCol = 999, maxCol = -999, minRow = 999, maxRow = -999;
        for (const auto& cell : nextBlock->getCells()) {
            if (cell.col < minCol) minCol = cell.col;
            if (cell.col > maxCol) maxCol = cell.col;
            if (cell.row < minRow) minRow = cell.row;
            if (cell.row > maxRow) maxRow = cell.row;
        }
        
        int blockWidth = (maxCol - minCol + 1) * cellSize;
        int blockHeight = (maxRow - minRow + 1) * cellSize;
        
        // Center the block in the preview area
        int previewCenterX = x + INFO_BOX_WIDTH / 2;
        int previewCenterY = y + 30 + 27;  // Middle of preview box
        int offsetX = previewCenterX - blockWidth / 2 - minCol * cellSize;
        int offsetY = previewCenterY - blockHeight / 2 - minRow * cellSize;
        
        for (const auto& cell : nextBlock->getCells()) {
            int px = offsetX + cell.col * cellSize;
            int py = offsetY + cell.row * cellSize;
            
            xw->fillRectangle(px + 1, py + 1, cellSize - 2, cellSize - 2, 
                             getColorForSymbol(nextBlock->getSymbol()));
            xw->drawRectangle(px, py, cellSize, cellSize, XWindow::Black);
        }
    }
}

void GraphicsObserver::drawHeldBlockBox(int x, int y, Block* heldBlock) {
    // Draw box - light gray background with dark border
    xw->fillRectangle(x, y, INFO_BOX_WIDTH, 95, XWindow::LightGray);
    xw->drawRectangle(x, y, INFO_BOX_WIDTH, 95, XWindow::Black);
    xw->drawRectangle(x - 1, y - 1, INFO_BOX_WIDTH + 2, 97, XWindow::Black);
    
    // Label - centered
    xw->drawString(x + 100, y + 20, "Held Block", XWindow::Black);
    
    // Preview area - slightly lighter  
    xw->fillRectangle(x + 10, y + 30, INFO_BOX_WIDTH - 20, 55, XWindow::White);
    xw->drawRectangle(x + 10, y + 30, INFO_BOX_WIDTH - 20, 55, XWindow::Black);
    
    // Draw the held block in the preview area
    if (heldBlock) {
        int cellSize = 12;  // Smaller cells for preview
        
        // Calculate block bounds to center it properly
        int minCol = 999, maxCol = -999, minRow = 999, maxRow = -999;
        for (const auto& cell : heldBlock->getCells()) {
            if (cell.col < minCol) minCol = cell.col;
            if (cell.col > maxCol) maxCol = cell.col;
            if (cell.row < minRow) minRow = cell.row;
            if (cell.row > maxRow) maxRow = cell.row;
        }
        
        int blockWidth = (maxCol - minCol + 1) * cellSize;
        int blockHeight = (maxRow - minRow + 1) * cellSize;
        
        // Center the block in the preview area
        int previewCenterX = x + INFO_BOX_WIDTH / 2;
        int previewCenterY = y + 30 + 27;  // Middle of preview box
        int offsetX = previewCenterX - blockWidth / 2 - minCol * cellSize;
        int offsetY = previewCenterY - blockHeight / 2 - minRow * cellSize;
        
        for (const auto& cell : heldBlock->getCells()) {
            int px = offsetX + cell.col * cellSize;
            int py = offsetY + cell.row * cellSize;
            
            xw->fillRectangle(px + 1, py + 1, cellSize - 2, cellSize - 2, 
                             getColorForSymbol(heldBlock->getSymbol()));
            xw->drawRectangle(px, py, cellSize, cellSize, XWindow::Black);
        }
    }
}

void GraphicsObserver::drawBoard(Board* board, int offsetX, int offsetY,
                                  Block* curBlock, const Position& curPos) {
    // Calculate cell size to fit board width exactly to match info box width
    int cellW = BOARD_WIDTH / board->getCols();  
    int cellH = BOARD_HEIGHT / board->getRows();
    
    // Draw board background with light gray color
    xw->fillRectangle(offsetX, offsetY, BOARD_WIDTH, BOARD_HEIGHT, XWindow::LightGray);
    
    // Draw thick dark border
    xw->drawRectangle(offsetX - 3, offsetY - 3, BOARD_WIDTH + 6, BOARD_HEIGHT + 6, XWindow::Black);
    xw->drawRectangle(offsetX - 4, offsetY - 4, BOARD_WIDTH + 8, BOARD_HEIGHT + 8, XWindow::Black);
    
    // Draw placed cells from board
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            char sym = board->getCell(r, c);
            int x = offsetX + c * cellW;
            int y = offsetY + r * cellH;
            
            if (sym == '?') {
                // Blind effect: draw with dark gray color and question mark pattern
                // Use a darker gray to indicate obscured area
                xw->fillRectangle(x + 1, y + 1, cellW - 2, cellH - 2, XWindow::DarkGray);
                // Draw a lighter border to make it visible
                xw->drawRectangle(x, y, cellW, cellH, XWindow::Black);
                // Draw question mark in the center
                int centerX = x + cellW / 2 - 4;
                int centerY = y + cellH / 2 + 4;
                xw->drawString(centerX, centerY, "?", XWindow::White);
            } else if (sym != ' ') {
                // Filled cell with color and thin black border
                xw->fillRectangle(x + 1, y + 1, cellW - 2, cellH - 2, 
                                 getColorForSymbol(sym));
                xw->drawRectangle(x, y, cellW, cellH, XWindow::Black);
            }
        }
    }
    
    // Draw phantom block (where the block will land) - only if enabled
    if (curBlock && showPhantom) {
        // Calculate phantom position by moving block down until it can't go further
        Position phantomPos = curPos;
        while (board->canPlace(*curBlock, Position{phantomPos.row + 1, phantomPos.col})) {
            phantomPos.row++;
        }
        
        // Only draw phantom if it's not at the same position as current block
        if (phantomPos.row != curPos.row) {
            for (const auto& cell : curBlock->getCells()) {
                int r = phantomPos.row + cell.row;
                int c = phantomPos.col + cell.col;
                
                if (r >= 0 && r < board->getRows() && c >= 0 && c < board->getCols()) {
                    // Check if this cell is in the blind area (should not be visible)
                    if (board->getCell(r, c) == '?') {
                        // Skip drawing phantom block in blind area
                        continue;
                    }
                    
                    int x = offsetX + c * cellW;
                    int y = offsetY + r * cellH;
                    
                    // Draw phantom block with white color (lighter appearance)
                    xw->fillRectangle(x + 1, y + 1, cellW - 2, cellH - 2, XWindow::White);
                    // Draw border with the actual block color for visibility
                    xw->drawRectangle(x, y, cellW, cellH, getColorForSymbol(curBlock->getSymbol()));
                }
            }
        }
    }
    
    // Draw current active block on top
    if (curBlock) {
        for (const auto& cell : curBlock->getCells()) {
            int r = curPos.row + cell.row;
            int c = curPos.col + cell.col;
            
            if (r >= 0 && r < board->getRows() && c >= 0 && c < board->getCols()) {
                // Check if this cell is in the blind area (should not be visible)
                if (board->getCell(r, c) == '?') {
                    // Skip drawing current block in blind area
                    continue;
                }
                
                int x = offsetX + c * cellW;
                int y = offsetY + r * cellH;
                
                xw->fillRectangle(x + 1, y + 1, cellW - 2, cellH - 2, 
                                 getColorForSymbol(curBlock->getSymbol()));
                xw->drawRectangle(x, y, cellW, cellH, XWindow::Black);
            }
        }
    }
}

XWindow* GraphicsObserver::getXWindow() {
    return xw.get();
}

void GraphicsObserver::togglePhantom() {
    showPhantom = !showPhantom;
}

void GraphicsObserver::showGameOver(int winner) {
    // Draw semi-transparent overlay effect
    for (int i = 0; i < 3; ++i) {
        xw->fillRectangle(60 + i, 220 + i, 540 - i*2, 180 - i*2, XWindow::Black);
    }
    
    // Draw Game Over box
    xw->fillRectangle(60, 220, 540, 180, XWindow::DarkGray);
    xw->drawRectangle(60, 220, 540, 180, XWindow::White);
    xw->drawRectangle(62, 222, 536, 176, XWindow::White);
    
    // Draw "GAME OVER" text (large-ish with multiple draws for emphasis)
    xw->drawString(220, 260, "G A M E  O V E R", XWindow::White);
    xw->drawString(221, 261, "G A M E  O V E R", XWindow::White);  // Shadow effect
    
    // Draw winner/tie message
    if (winner == 1) {
        xw->drawString(255, 295, "Player 1 Wins!", XWindow::Cyan);
        xw->drawString(256, 296, "Player 1 Wins!", XWindow::Cyan);
    } else if (winner == 2) {
        xw->drawString(255, 295, "Player 2 Wins!", XWindow::Magenta);
        xw->drawString(256, 296, "Player 2 Wins!", XWindow::Magenta);
    } else {
        xw->drawString(270, 295, "Both Players Lost", XWindow::White);
    }
    
    // Draw instructions
    xw->drawString(210, 340, "Type 'restart' or 'quit'", XWindow::White);
}


