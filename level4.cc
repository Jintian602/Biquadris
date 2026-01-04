// Level4 module - interface

export module level4;

import level3;
import block;
import <memory>;
import <string>;

/*
 * Level4:
 * -------
 * Inherits all rules from Level3:
 *   - Same biased distribution (S and Z: 2/9 each, others: 1/9 each)
 *   - Heavy blocks (auto-drop 1 row after move/rotate)
 *
 * Additional game behaviour for Level4 (e.g., dropping 1x1 '*' blocks)
 * is implemented outside of Level4 in the Board/Player logic.
 *
 * Level4 itself only controls block-type generation.
 * Script files are not used at this level.
 */
export class Level4 : public Level3 {
public:
    // Construct Level4 (levelNum = 4)
    Level4();

    // Create a concrete Block instance
    std::shared_ptr<Block> generateBlock(int id) override;

    // Return only the next block type
    char generateBlock() override;

    // Enable or disable random generation
    void setRandom(bool random) override;

    // Script files are unused in Level4
    void setScriptFile(const std::string& filename) override;
};

