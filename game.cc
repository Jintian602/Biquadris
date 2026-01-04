/**
 * @file game.cc
 * @brief Interface for the Game class (main game controller)
 * 
 * This file defines the Game class, which is the central controller for
 * the Biquadris game. It manages two players and the overall game flow.
 * Note: Game does NOT handle display - that's done via the Observer pattern
 * where Observers are attached to Boards directly.
 */

export module game;

import <memory>;
import <string>;
import player;

/**
 * @class Game
 * @brief Main game controller managing game state and player turns
 * 
 * The Game class coordinates the Biquadris game logic:
 * - Manages two Player objects (ownership via shared_ptr)
 * - Handles command execution for current player
 * - Controls turn switching between players
 * - Manages game over conditions and restart
 * 
 * According to UML design, Game does NOT know about Observers.
 * Display is handled through the Observer pattern where TextObserver
 * and GraphicsObserver are attached to Board objects in main().
 */
export class Game {
    std::shared_ptr<Player> p1;  ///< Player 1 (ownership)
    std::shared_ptr<Player> p2;  ///< Player 2 (ownership)
    int current = 1;             ///< Current player (1 or 2)
    
public:
    /**
     * @brief Constructs a new Game with two players
     * 
     * Initializes both players. Observers should be attached to
     * the player boards externally (in main).
     */
    Game();
    
    /**
     * @brief Starts the game
     * 
     * Called once at the beginning. Initial display happens automatically
     * through the Observer pattern when boards are first updated.
     */
    void run();
    
    /**
     * @brief Gets a pointer to player 1
     * @return Shared pointer to player 1
     */
    std::shared_ptr<Player> getPlayer1();
    
    /**
     * @brief Gets a pointer to player 2
     * @return Shared pointer to player 2
     */
    std::shared_ptr<Player> getPlayer2();
    
    /**
     * @brief Gets a pointer to the current player
     * @return Shared pointer to the active player
     */
    std::shared_ptr<Player> getCurrentPlayer();
    
    /**
     * @brief Switches the active player (1 -> 2 or 2 -> 1)
     * 
     * Called after certain commands (like "drop") to alternate turns.
     */
    void switchTurn();
    
    /**
     * @brief Executes a game command for the current player
     * @param cmd The command string (e.g., "left", "right", "drop")
     * @param multiplier Number of times to execute the command
     * @param droppingPlayerNum Output parameter: player number who dropped (1 or 2), set only if special effect should be applied
     * @param blockDropped Output parameter: set to true if a block was dropped (either via drop command or auto-drop)
     * @return true if a special effect should be applied (2+ rows cleared), false otherwise
     * 
     * This method handles all player actions. After actions that modify
     * the board, the Board automatically notifies its observers.
     */
    bool handleCommand(const std::string& cmd, int multiplier = 1, int* droppingPlayerNum = nullptr, bool* blockDropped = nullptr);
    
    /**
     * @brief Checks if the game has ended
     * @return true if either player has lost, false otherwise
     */
    bool isGameOver();
    
    /**
     * @brief Restarts the game with fresh boards and blocks
     * 
     * Resets both players to initial state. Display updates happen
     * automatically through the Observer pattern.
     */
    void restart();
    
    /**
     * @brief Applies a special effect to the opponent
     * @param effect The effect type: "blind", "heavy", or "force <blockType>"
     * @param targetPlayer The player number to apply effect to (1 or 2). If 0, applies to opponent of current player.
     * @return true if effect was successfully applied, false otherwise
     * 
     * This is called when a player clears 2+ rows and chooses a special effect.
     * The effect is applied to the specified player using the decorator pattern.
     * If the effect is invalid, outputs instructions and returns false.
     */
    bool applySpecialEffect(const std::string& effect, int targetPlayer = 0);
    
    /**
     * @brief Checks if the current player can apply a special effect
     * @return true if 2+ rows were cleared in the last drop
     */
    bool canApplySpecial() const;
};


