/**
 * @file graphicsobserver.cc
 * @brief Interface for the GraphicsObserver class
 * 
 * This file defines the GraphicsObserver class, which implements the Observer
 * pattern to display the game state using a graphical window (XWindow).
 * It observes two game boards and updates the display whenever notified.
 */

export module graphicsobserver;

import textobserver;
import player;
import board;
import block;
import position;
import xwindow;
import <memory>;

/**
 * @class GraphicsObserver
 * @brief Observer that renders the game state graphically using XWindow
 * 
 * This class inherits from TextObserver and provides a graphical representation
 * of the Biquadris game. It displays two player boards side-by-side, along
 * with their scores, levels, and preview boxes for next/held blocks.
 * 
 * Since all graphics need text information, GraphicsObserver extends TextObserver
 * to avoid duplicating the common fields and methods.
 */
export class GraphicsObserver : public TextObserver {
    // Graphics-specific: XWindow for rendering (owned)
    std::shared_ptr<XWindow> xw;
    
    // Display constants (layout dimensions in pixels)
    static const int CELL_SIZE = 26;        ///< Size of each cell in pixels
    static const int BOARD_WIDTH = 290;     ///< Width of game board display
    static const int BOARD_HEIGHT = 470;    ///< Height of game board display
    static const int MARGIN = 20;           ///< Margin from window edges
    static const int PLAYER_SPACING = 320;  ///< Horizontal space between players
    static const int INFO_BOX_WIDTH = 290;  ///< Width of info/preview boxes
    static const int INFO_BOX_HEIGHT = 55;  ///< Height of player info box
    
    // Flag to control phantom block display
    bool showPhantom;
    
    /**
     * @brief Maps a block symbol to its corresponding color
     * @param sym The character symbol representing a block type
     * @return Integer representing the XWindow color constant
     */
    int getColorForSymbol(char sym);
    
    /**
     * @brief Draws a game board with its current and falling blocks
     * @param board Pointer to the Board to render
     * @param offsetX X-coordinate for board placement
     * @param offsetY Y-coordinate for board placement
     * @param curBlock Pointer to the currently falling block (or nullptr)
     * @param curPos Position of the current block
     */
    void drawBoard(Board* board, int offsetX, int offsetY,
                   Block* curBlock, const Position& curPos);
    
    /**
     * @brief Draws the player information box (level and score)
     * @param x X-coordinate of the box
     * @param y Y-coordinate of the box
     * @param playerNum Player number (1 or 2)
     * @param level Current level of the player
     * @param score Current score of the player
     */
    void drawInfoBox(int x, int y, int playerNum, int level, int score);
    
    /**
     * @brief Draws the "Next Block" preview box
     * @param x X-coordinate of the box
     * @param y Y-coordinate of the box
     * @param nextBlock Pointer to the next block to display (or nullptr)
     */
    void drawNextBlockBox(int x, int y, Block* nextBlock);
    
    /**
     * @brief Draws the "Held Block" preview box
     * @param x X-coordinate of the box
     * @param y Y-coordinate of the box
     * @param heldBlock Pointer to the held block to display (or nullptr)
     */
    void drawHeldBlockBox(int x, int y, Block* heldBlock);
    
public:
    /**
     * @brief Constructs a GraphicsObserver for two players
     * @param p1 Pointer to player 1 (non-owning, Subject)
     * @param p2 Pointer to player 2 (non-owning, Subject)
     */
    GraphicsObserver(Player* p1, Player* p2);
    
    /**
     * @brief Called when the observed subject changes state
     * 
     * This method is part of the Observer pattern. It triggers a redraw
     * of the entire game display.
     */
    void notify() override;
    
    // Inherits player1 and player2 from TextObserver
    // All information is obtained directly from Player (Subject) in draw()
    
    /**
     * @brief Renders the complete game state to the window
     * 
     * This method draws all UI elements including high score bar,
     * player info boxes, game boards, and preview boxes.
     */
    void draw();
    
    /**
     * @brief Gets direct access to XWindow for event handling (raw pointer)
     * @return Raw pointer to the XWindow instance
     */
    XWindow* getXWindow();
    
    /**
     * @brief Displays "Game Over" message on the graphics window
     * @param winner Player number (1 or 2) or 0 for tie
     */
    void showGameOver(int winner = 0);
    
    /**
     * @brief Toggles the display of phantom blocks
     * 
     * Toggles whether phantom blocks (showing where
     * the current block will land) are displayed.
     */
    void togglePhantom();
};


