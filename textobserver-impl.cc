/**
 * @file textobserver-impl.cc
 * @brief Implementation of the TextObserver class
 * 
 * This file contains the implementation of all TextObserver methods,
 * including the text-based rendering logic for displaying the game state
 * on the console with proper formatting and layout.
 */

module textobserver;

import observer;
import player;
import board;
import block;
import position;
import <iostream>;
import <string>;
import <vector>;
import <memory>;

TextObserver::TextObserver(Player* p1, Player* p2) 
    : player1{p1}, player2{p2} {}

void TextObserver::notify() {
    display();
}

// Helper function to get block lines for display
std::vector<std::string> getBlockLines(Block* block) {
    using namespace std;
    vector<string> lines;
    
    if (!block) {
        lines.push_back(" ");
        return lines;
    }
    
    const auto& cells = block->getCells();
    if (cells.empty()) {
        lines.push_back(" ");
        return lines;
    }
    
    // Find bounding box
    int minR = cells[0].row, maxR = cells[0].row;
    int minC = cells[0].col, maxC = cells[0].col;
    
    for (const auto& cell : cells) {
        if (cell.row < minR) minR = cell.row;
        if (cell.row > maxR) maxR = cell.row;
        if (cell.col < minC) minC = cell.col;
        if (cell.col > maxC) maxC = cell.col;
    }
    
    // Create grid for this block
    for (int r = minR; r <= maxR; ++r) {
        string line;
        for (int c = minC; c <= maxC; ++c) {
            bool found = false;
            for (const auto& cell : cells) {
                if (cell.row == r && cell.col == c) {
                    line += block->getSymbol();
                    found = true;
                    break;
                }
            }
            if (!found) line += ' ';
        }
        lines.push_back(line);
    }
    
    return lines;
}

void TextObserver::display() {
    using namespace std;
    
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
    
    // Display level and score info for both players
    cout << "Level:    " << level1 << "            Level:    " << level2 << endl;
    cout << "Score:    " << score1 << "            Score:    " << score2 << endl;
    cout << "-----------            -----------" << endl;
    
    // Create temporary grids to overlay current blocks
    vector<vector<char>> display1(board1->getRows(), vector<char>(board1->getCols()));
    vector<vector<char>> display2(board2->getRows(), vector<char>(board2->getCols()));
    
    // Copy board state
    for (int r = 0; r < board1->getRows(); ++r) {
        for (int c = 0; c < board1->getCols(); ++c) {
            display1[r][c] = board1->getCell(r, c);
            display2[r][c] = board2->getCell(r, c);
        }
    }
    
    // Overlay current block for player 1
    if (curBlock1) {
        for (const auto& cell : curBlock1->getCells()) {
            int r = curPos1.row + cell.row;
            int c = curPos1.col + cell.col;
            if (r >= 0 && r < board1->getRows() && c >= 0 && c < board1->getCols()) {
                display1[r][c] = curBlock1->getSymbol();
            }
        }
    }
    
    // Overlay current block for player 2
    if (curBlock2) {
        for (const auto& cell : curBlock2->getCells()) {
            int r = curPos2.row + cell.row;
            int c = curPos2.col + cell.col;
            if (r >= 0 && r < board2->getRows() && c >= 0 && c < board2->getCols()) {
                display2[r][c] = curBlock2->getSymbol();
            }
        }
    }
    
    // Display both boards side by side
    for (int r = 0; r < board1->getRows(); ++r) {
        for (int c = 0; c < board1->getCols(); ++c) {
            cout << display1[r][c];
        }
        cout << "            ";  // 12 spaces
        for (int c = 0; c < board2->getCols(); ++c) {
            cout << display2[r][c];
        }
        cout << endl;
    }
    
    cout << "-----------            -----------" << endl;
    
    // Display next blocks
    cout << "Next:                  Next:" << endl;
    
    vector<string> next1Lines = getBlockLines(nextBlock1.get());
    vector<string> next2Lines = getBlockLines(nextBlock2.get());
    
    size_t maxLines = max(next1Lines.size(), next2Lines.size());
    for (size_t i = 0; i < maxLines; ++i) {
        // Display next block 1
        if (i < next1Lines.size()) {
            cout << next1Lines[i];
            // Pad to reach player 2's column
            for (size_t j = next1Lines[i].length(); j < 23; ++j) {
                cout << ' ';
            }
        } else {
            for (int j = 0; j < 23; ++j) cout << ' ';
        }
        
        // Display next block 2
        if (i < next2Lines.size()) {
            cout << next2Lines[i];
        }
        cout << endl;
    }
    
    // Display held blocks if they exist
    if (heldBlock1 || heldBlock2) {
        cout << "Held:                  Held:" << endl;
        
        vector<string> held1Lines = getBlockLines(heldBlock1.get());
        vector<string> held2Lines = getBlockLines(heldBlock2.get());
        
        maxLines = max(held1Lines.size(), held2Lines.size());
        for (size_t i = 0; i < maxLines; ++i) {
            // Display held block 1
            if (i < held1Lines.size()) {
                cout << held1Lines[i];
                for (size_t j = held1Lines[i].length(); j < 23; ++j) {
                    cout << ' ';
                }
            } else {
                for (int j = 0; j < 23; ++j) cout << ' ';
            }
            
            // Display held block 2
            if (i < held2Lines.size()) {
                cout << held2Lines[i];
            }
            cout << endl;
        }
    }
}


