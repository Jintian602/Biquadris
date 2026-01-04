/**
 * @file commandinterpreter.cc
 * @brief Interface for the CommandInterpreter class
 * 
 * This file defines the CommandInterpreter class, which is responsible for
 * parsing and interpreting user commands in the Biquadris game. It handles
 * command abbreviations and multipliers (e.g., "3 left" means move left 3 times).
 */

export module commandinterpreter;

import <string>;

/**
 * @class CommandInterpreter
 * @brief Parses and interprets user input commands for game control
 * 
 * The CommandInterpreter class processes raw user input strings and converts
 * them into commands that the game can execute. It supports:
 * - Command abbreviation matching (e.g., "lef" matches "left")
 * - Command multipliers (e.g., "3 right" executes "right" 3 times)
 * - Command normalization (converting to lowercase)
 */
export class CommandInterpreter {
public:
    /**
     * @brief Parses an input string into a command and multiplier
     * @param input The raw input string from the user
     * @param cmd Output parameter: the parsed command string
     * @param multiplier Output parameter: number of times to repeat the command
     * @return true if parsing was successful, false otherwise
     * 
     * This method extracts the command and optional multiplier from the input.
     * If the input starts with a number, that number becomes the multiplier.
     * The command is converted to lowercase for case-insensitive matching.
     * 
     * Examples:
     * - "left" -> cmd="left", multiplier=1
     * - "3 right" -> cmd="right", multiplier=3
     * - "DROP" -> cmd="drop", multiplier=1
     */
    bool nextCommand(const std::string& input, std::string& cmd, int& multiplier);
    
    /**
     * @brief Matches abbreviated commands to their full form
     * @param input The abbreviated or full command string
     * @return The matched full command string, or the original if no match
     * 
     * This static method implements command abbreviation by finding the
     * shortest unique prefix. For example, "lef" matches "left" because
     * it uniquely identifies that command.
     * 
     * Special cases:
     * - Single-character commands: I, J, L, O, S, T, Z (testing blocks)
     * - "clockwise" is abbreviated to "cw"
     * - "counterclockwise" is abbreviated to "ccw"
     * 
     * Command list:
     * - Movement: left, right, down
     * - Rotation: clockwise (cw), counterclockwise (ccw)
     * - Actions: drop, levelup, leveldown
     * - Special: restart, hint, norandom, random, sequence
     * - Testing: I, J, L, O, S, T, Z
     */
    static std::string matchCommand(const std::string& input);
};


