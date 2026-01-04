/**
 * @file textobserver.cc
 * @brief Interface for the TextObserver class
 * 
 * This file defines the TextObserver class, which implements the Observer
 * pattern to display the game state in text mode using standard output.
 * It observes two game boards and updates the console display when notified.
 */

export module textobserver;

import observer;
import player;
import board;

/**
 * @class TextObserver
 * @brief Observer that renders the game state as text to the console
 * 
 * This class inherits from Observer and provides a text-based representation
 * of the Biquadris game. It displays both player boards side-by-side with
 * ASCII borders, along with scores, levels, and preview sections.
 * 
 * Observer observes Player (Subject) and gets all information from Player,
 * including Board, Block, Position, Score, and Level. This follows proper
 * Observer pattern design where Observer only accesses Subject.
 */
export class TextObserver : public Observer {
protected:
    // Protected members so subclasses (like GraphicsObserver) can access them
    Player* player1;    ///< Pointer to player 1 (non-owning, Subject)
    Player* player2;    ///< Pointer to player 2 (non-owning, Subject)
    
public:
    /**
     * @brief Constructs a TextObserver for two players
     * @param p1 Pointer to player 1 (non-owning, Subject)
     * @param p2 Pointer to player 2 (non-owning, Subject)
     */
    TextObserver(Player* p1, Player* p2);
    
    /**
     * @brief Called when the observed subject changes state
     * 
     * This method is part of the Observer pattern. It triggers a refresh
     * of the text display by calling the display() method.
     * Gets all information directly from Player (Subject).
     */
    void notify() override;
    
    /**
     * @brief Renders the complete game state to standard output
     * 
     * This method gets all information from Player and prints the game
     * boards, player info, and UI elements to the console using ASCII
     * characters and formatting.
     */
    void display();
};


