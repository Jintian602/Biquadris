// Level0 module - interface
export module level0;

import level;
import block;
import <memory>;
import <string>;
import <vector>;

/*
 * Level0:
 * -------
 * Deterministic level used only for scripted block sequences.
 *
 * Block generation in Level0 is NOT random.  Instead, it reads a
 * sequence of block types from a script file (default "sequence.txt")
 * and cycles through them repeatedly.  This is used for testing and for
 * deterministic gameplay behaviour.
 */
export class Level0 : public Level {
    std::string scriptFile;     // Path to the script file containing block types
    std::vector<char> sequence; // Sequence of block types read from file
    int currentIndex = 0;       // Current position in sequence
    
    /*
     * loadSequence():
     * ----------------
     * Reads characters from scriptFile and fills the sequence vector.
     * Valid block types are I,J,L,O,S,T,Z.  Characters are consumed
     * exactly as stored in the file.  If the file is empty, Level0
     * defaults to producing 'I'.
     */
    void loadSequence();
    
public:
    Level0();
    
    /*
     * generateBlock(int id):
     * -----------------------
     * Creates and returns a Block object using the next block type
     * from the sequence.  Wraps around to the start of the sequence
     * when the end is reached.
     */
    std::shared_ptr<Block> generateBlock(int id) override;

    /*
     * generateBlock():
     * ----------------
     * Returns the next block type character from the sequence.
     */
    char generateBlock() override;

    /*
     * setRandom(bool):
     * ------------------
     * Level0 never uses randomness.  This method intentionally does nothing.
     */
    void setRandom(bool random) override;

    /*
     * setScriptFile(const std::string&):
     * -----------------------------------
     * Updates the script file path and reloads the sequence from disk.
     */
    void setScriptFile(const std::string& filename) override;
};


