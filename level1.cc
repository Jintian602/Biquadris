// Level1 module - interface

export module level1;

import level;
import block;
import <memory>;
import <string>;

/*
 * Level1:
 * Random level with skewed probabilities.
 *   - S, Z appear less often.
 *   - I, J, L, O, T appear twice as often.
 */
export class Level1 : public Level {
    bool randomMode = true;  // Whether randomness is enabled

public:
    // Construct Level1 (random behaviour configured globally via std::srand).
    Level1();

    // Generate a Block object with the Level1 distribution.
    std::shared_ptr<Block> generateBlock(int id) override;

    // Generate just the block type ('I','J','L','O','S','T','Z').
    char generateBlock() override;

    // Turn randomness on/off for test scenarios.
    void setRandom(bool random) override;

    // Level1 ignores script files.
    void setScriptFile(const std::string &filename) override;
};


