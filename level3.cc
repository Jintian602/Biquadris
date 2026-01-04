// Level3 module - interface
export module level3;

import level;
import block;
import <memory>;
import <string>;

/*
 * Level3:
 * -------
 * Random block generation with biased probabilities:
 *   - S and Z appear more frequently than other blocks.
 *   - Does NOT use script files.
 *   - randomMode can be toggled, e.g. by norandom/random commands.
 *
 * Heavy gameplay behavior is NOT handled here — it is applied
 * by Player/HeavyEffect according to level number.
 */
export class Level3 : public Level {
    bool randomMode = true;  // whether randomness is enabled

public:
    // Construct Level3 (levelNum = 3)
    Level3();

    // Create a concrete Block using the generated type
    std::shared_ptr<Block> generateBlock(int id) override;

    // Return only the block type (S,Z,I,J,L,O,T)
    char generateBlock() override;

    // Enable or disable random generation
    void setRandom(bool random) override;

    // Level3 does not use script files — method has no effect
    void setScriptFile(const std::string& filename) override;
};

