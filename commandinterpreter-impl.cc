/**
 * @file commandinterpreter-impl.cc
 * @brief Implementation of the CommandInterpreter class
 * 
 * This file contains the implementation of command parsing logic, including
 * input parsing, multiplier extraction, lowercase conversion, and command
 * abbreviation matching.
 */

module commandinterpreter;

import <string>;
import <iostream>;
import <sstream>;

/**
 * @brief Helper function to check if a character is a digit
 * @param c The character to check
 * @return true if c is between '0' and '9', false otherwise
 */
static bool isDigitChar(char c) {
    return c >= '0' && c <= '9';
}

/**
 * @brief Helper function to convert a character to lowercase
 * @param c The character to convert
 * @return The lowercase version of c, or c itself if not a letter
 */
static char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

bool CommandInterpreter::nextCommand(const std::string& input, std::string& cmd, int& multiplier) {
    multiplier = 1;
    cmd = "";
    
    std::istringstream iss{input};
    std::string first;
    
    if (!(iss >> first)) return false;
    
    if (isDigitChar(first[0])) {
        multiplier = std::stoi(first);
        if (!(iss >> cmd)) return false;
    } else {
        cmd = first;
    }
    
    for (char& c : cmd) {
        c = toLowerChar(c);
    }
    
    return true;
}

std::string CommandInterpreter::matchCommand(const std::string& input) {
    // Single-character testing commands (used for testing specific blocks)
    if (input.length() == 1) {
        char c = input[0];
        // I, J, L, O, S, T, Z are valid testing block commands
        if (c == 'I' || c == 'i' || c == 'J' || c == 'j' || 
            c == 'L' || c == 'l' || c == 'O' || c == 'o' || 
            c == 'S' || c == 's' || c == 'T' || c == 't' || 
            c == 'Z' || c == 'z') {
            // Return uppercase for consistency
            if (c >= 'a' && c <= 'z') {
                c = c - ('a' - 'A');
            }
            return std::string(1, c);
        }
    }
    
    // List of all multi-character game commands
    const std::string commands[] = {
        "left", "right", "down", "clockwise", "counterclockwise",
        "drop", "hold", "levelup", "leveldown", "norandom", "random",
        "sequence", "restart", "hint", "cw", "ccw", "phantom"
    };
    
    // Count how many commands match the input prefix
    int matchCount = 0;
    std::string matchedCommand;
    
    // Try to match input as a prefix of each command (shortest unique prefix)
    for (const auto& cmd : commands) {
        if (cmd.find(input) == 0) {
            matchCount++;
            if (matchCount == 1) {
                // Store the first match
                matchedCommand = cmd;
            } else {
                // Multiple matches found - return original input (invalid command)
                return input;
            }
        }
    }
    
    // If exactly one match found, return it
    if (matchCount == 1) {
        // Special abbreviations: convert full names to short forms
        if (matchedCommand == "clockwise") return "cw";
        if (matchedCommand == "counterclockwise") return "ccw";
        return matchedCommand;
    }
    
    // If no match found, return the original input unchanged
    return input;
}


