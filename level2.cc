// Level2 module - interface

export module level2;

import level;
import block;
import <memory>;
import <string>;

/*
 * Level2:
 * Random level with uniform distribution over all 7 pieces.
 */
export class Level2 : public Level {
    bool randomMode = true;  // Whether randomness is enabled

public:
    Level2();

    // Create a Block with uniform probability over block types.
    std::shared_ptr<Block> generateBlock(int id) override;

    // Return a randomly chosen block type.
    char generateBlock() override;

    // Enable/disable randomness for testing.
    void setRandom(bool random) override;

    // Level2 ignores script files.
    void setScriptFile(const std::string &filename) override;
};

