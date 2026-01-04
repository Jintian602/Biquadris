import <iostream>;
import <string>;
import <sstream>;
import <memory>;
import <cstdlib>;
import game;
import commandinterpreter;
import position;
import textobserver;
import graphicsobserver;

using namespace std;

int main(int argc, char* argv[]) {
    try {
        string scriptFile1 = "biquadris_sequence1.txt";
        string scriptFile2 = "biquadris_sequence2.txt";
        int startLevel = 0;
        bool textOnly = false;
        unsigned int seed = 0;
        bool useSeed = false;
        bool enableStdin = false;  // Enable stdin input in graphics mode
        
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];
            if (arg == "-text") {
                textOnly = true;
            } else if (arg == "-seed" && i + 1 < argc) {
                seed = stoi(argv[++i]);
                useSeed = true;
            } else if (arg == "-scriptfile1" && i + 1 < argc) {
                scriptFile1 = argv[++i];
            } else if (arg == "-scriptfile2" && i + 1 < argc) {
                scriptFile2 = argv[++i];
            } else if (arg == "-startlevel" && i + 1 < argc) {
                startLevel = stoi(argv[++i]);
                if (startLevel < 0) startLevel = 0;
                if (startLevel > 4) startLevel = 4;
            } else if (arg == "-enableStdin" || arg == "-disableKey") {
                enableStdin = true;  // Enable stdin input even in graphics mode
            }
        }
        
        // Set random seed if specified
        if (useSeed) {
            srand(seed);
        }
        
        // Create game (manages players only)
        auto game = make_shared<Game>();
        
        // Set start levels and script files for players
        for (int i = 0; i < startLevel; ++i) {
            game->getPlayer1()->levelUp();
            game->getPlayer2()->levelUp();
        }
        
        // Set script files for Level 0
        game->getPlayer1()->setScriptFile(scriptFile1);
        game->getPlayer2()->setScriptFile(scriptFile2);
        
        // Generate initial next blocks AFTER levels and script files are configured
        game->getPlayer1()->generateNextBlock();
        game->getPlayer2()->generateNextBlock();
        
        // Create observers and attach them to players (Observer pattern)
        // Observer observes Player (Subject) and gets all information from Player
        auto textObs = make_shared<TextObserver>(
            game->getPlayer1().get(),
            game->getPlayer2().get()
        );
        game->getPlayer1()->attach(textObs.get());
        game->getPlayer2()->attach(textObs.get());
        
        shared_ptr<GraphicsObserver> graphicsObs;
        if (!textOnly) {
            graphicsObs = make_shared<GraphicsObserver>(
                game->getPlayer1().get(),
                game->getPlayer2().get()
            );
            game->getPlayer1()->attach(graphicsObs.get());
            game->getPlayer2()->attach(graphicsObs.get());
        }
        
        cout << "Welcome to Biquadris!" << endl;
        cout << "Commands:" << endl;
        cout << "  Movement: left, right, down" << endl;
        cout << "  Rotation: clockwise (cw), counterclockwise (ccw)" << endl;
        cout << "  Actions: drop, hold, levelup, leveldown" << endl;
        cout << "  Special: restart, hint, norandom, random, sequence" << endl;
        cout << "  Testing: I, J, L, O, S, T, Z" << endl;
        cout << endl;
        cout << "Keyboard Shortcuts (Graphics Mode):" << endl;
        cout << "  Arrow Keys: ← → ↓ (move), ↑ (rotate CW)" << endl;
        cout << "  WASD: W=rotate, A=left, S=down, D=right" << endl;
        cout << "  Z/X: Z=rotate CCW, X=rotate CW" << endl;
        cout << "  H/C: Hold current block" << endl;
        cout << "  Space/Enter: Drop" << endl;
        cout << "  +/-: Level up/down" << endl;
        cout << "  P: Toggle phantom block display" << endl;
        cout << "  R: Restart, Q/Esc: Quit" << endl;
        cout << endl;
        cout << "Prefix commands with a number to repeat (e.g., '3right')" << endl;
        cout << "Abbreviations work (e.g., 'lef' for 'left')" << endl;
        cout << endl;
        
        game->run();
        
        // Initial display update - Observer gets all info from Player
        textObs->notify();
        
        if (graphicsObs) {
            graphicsObs->notify();
        }
        
        string input;
        while (true) {
            string cmd;
            int multiplier = 1;
            bool hasCommand = false;
            
            // Check for X11 keyboard events first (if in graphics mode)
            // This is always non-blocking, so it can work alongside stdin
            if (graphicsObs) {
                string key;
                if (graphicsObs->getXWindow()->checkEvent(key)) {
                    cmd = key;
                    hasCommand = true;
                    
                    // Handle quit command immediately (exit game loop)
                    if (cmd == "quit") {
                        break;
                    }
                }
            }
            
            // If no keyboard event, read from stdin
            // In graphics mode with -enableStdin: both X11 events and stdin work simultaneously
            // X11 events are checked first (non-blocking), then stdin (blocking if enabled)
            // In text-only mode: always read from stdin
            if (!hasCommand) {
                if (!graphicsObs || enableStdin) {
                    // Text-only mode or graphics mode with stdin enabled: read from stdin
                    // Note: This will block, but X11 events are checked first each iteration
                    if (cin >> input) {
                        // Handle quit command
                        if (input == "quit") break;
                        
                        // Check if input starts with a digit (multiplier prefix)
                        if (!input.empty() && input[0] >= '0' && input[0] <= '9') {
                            // Extract multiplier from beginning of string
                            size_t pos = 0;
                            while (pos < input.length() && input[pos] >= '0' && input[pos] <= '9') {
                                pos++;
                            }
                            multiplier = stoi(input.substr(0, pos));
                            cmd = input.substr(pos);
                            
                            // If no command after number, try to read next word
                            if (cmd.empty() && !(cin >> cmd)) {
                                continue;
                            }
                        } else {
                            cmd = input;
                        }
                        hasCommand = true;
                    } else {
                        // EOF or error - quit the game
                        break;
                    }
                } else {
                    // Graphics mode without stdin enabled: don't block, just continue
                    // This allows the X11 event checking to happen continuously
                    continue;
                }
            }
            
            // Match abbreviated command to full command
            cmd = CommandInterpreter::matchCommand(cmd);
            
            // Handle special commands that ignore multipliers
            if (cmd == "restart" || cmd == "hint" || cmd == "norandom" || 
                cmd == "random" || cmd == "sequence" || cmd == "phantom") {
                if (cmd == "restart") {
                    game->restart();
                    cout << "Game restarted!" << endl;
                } else if (cmd == "phantom") {
                    // Toggle phantom block display (graphics only)
                    if (graphicsObs) {
                        graphicsObs->togglePhantom();
                        graphicsObs->notify();  // Update display
                        cout << "Phantom block display toggled." << endl;
                    } else {
                        cout << "Phantom command only works in graphics mode." << endl;
                    }
                } else {
                    // Other special commands handled by game
                    game->handleCommand(cmd, 1);
                }
            } else {
                // Apply multiplier for regular commands
                // Treat 0 as 1 (execute once)
                if (multiplier <= 0) multiplier = 1;
                int droppingPlayer = 0;
                bool blockDropped = false;
                bool shouldApplySpecial = game->handleCommand(cmd, multiplier, &droppingPlayer, &blockDropped);
                
                // Handle turn switching and block spawning after a block is dropped
                // This is the responsibility of main.cc, not handleCommand
                if (blockDropped) {
                    // Block was dropped (either via drop command or auto-drop), switch turn and spawn for next player
                    game->switchTurn();
                    auto nextPlayer = game->getCurrentPlayer();
                    if (!nextPlayer->spawnBlock()) {
                        // Spawn failed - game over for this player
                    }
                }
                
                // Check if special effect should be applied after drop
                // handleCommand returns true if 2+ rows were cleared
                if (cmd == "drop" && shouldApplySpecial && droppingPlayer > 0) {
                    cout << "Special action available! You cleared 2+ rows." << endl;
                    
                    // Determine opponent: if droppingPlayer is 1, apply to 2; if 2, apply to 1
                    int targetPlayer = (droppingPlayer == 1) ? 2 : 1;
                    
                    bool effectApplied = false;
                    while (!effectApplied) {
                        cout << "Choose an effect to apply to opponent:" << endl;
                        cout << "  blind - Blind opponent's board (columns 3-9, rows 3-12)" << endl;
                        cout << "  heavy - Make opponent's blocks heavy (auto-drop 2 rows after move/rotate)" << endl;
                        cout << "  force <blockType> - Force opponent's current block type (e.g., force Z)" << endl;
                        cout << "Valid block types: I, J, L, O, S, T, Z" << endl;
                        cout << "Enter your choice (e.g., 'blind', 'heavy', or 'force Z'): ";
                        
                        string effectInput;
                        if (!(cin >> effectInput)) {
                            break;  // EOF or error
                        }
                        
                        // Handle "force" command with space-separated block type
                        if (effectInput == "force") {
                            string blockType;
                            if (cin >> blockType) {
                                effectInput = "force " + blockType;
                            } else {
                                effectInput = "force";  // Will trigger error message
                            }
                        }
                        
                        // Apply effect to the correct target player
                        effectApplied = game->applySpecialEffect(effectInput, targetPlayer);
                        
                        if (effectApplied) {
                            cout << "Effect applied successfully!" << endl;
                        } else {
                            cout << "Please try again." << endl;
                        }
                    }
                }
            }
            
            // Check for game over before updating displays
            if (game->isGameOver()) {
                cout << "Game Over!" << endl;
                
                // Determine winner
                int winner = 0;
                if (!game->getPlayer1()->isAlive() && game->getPlayer2()->isAlive()) {
                    winner = 2;
                    cout << "Player 2 Wins!" << endl;
                } else if (game->getPlayer1()->isAlive() && !game->getPlayer2()->isAlive()) {
                    winner = 1;
                    cout << "Player 1 Wins!" << endl;
                } else {
                    cout << "Both players lost!" << endl;
                }
                
                // Show Game Over in graphics
                if (graphicsObs) {
                    graphicsObs->showGameOver(winner);
                }
                
                cout << "Type 'restart' to play again or 'quit' to exit." << endl;
                
                // Don't update blocks after game over - wait for restart or quit
                continue;
            }
            
            // Observer gets all info from Player (Subject) automatically
            // Player will notify observers when state changes, but we can also
            // manually trigger update after commands if needed
            textObs->notify();  // Trigger text display update
            
            if (graphicsObs) {
                graphicsObs->notify();  // Trigger graphics display update
            }
        }
        
        cout << "Thanks for playing Biquadris!" << endl;
        
    } catch (const char* msg) {
        cerr << "Error: " << msg << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred" << endl;
        return 1;
    }
    
    return 0;
}


