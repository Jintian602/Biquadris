/**
 * @file level.cc
 * @brief Interface for the Level class (abstract level interface)
 *
 * Defines the abstract Level class.  Concrete levels (0–4) implement
 * different block-generation rules.
 */

 export module level;

 import <memory>;
 import <string>;
 import block;
 
 /**
  * @class Level
  * @brief Abstract base class representing a difficulty level
  *
  * Each Level object knows its level number (0–4) and provides an
  * interface for generating blocks according to that level's rules.
  */
 export class Level {
 protected:
     int levelNum;    ///< Level number (0–4)
 
 public:
     /// Construct a level with a given level number.
     Level(int num);
 
     /// Virtual destructor so derived levels can clean up correctly.
     virtual ~Level();
 
     /// Return the numeric level (0–4).
     int getLevelNum() const;
 
     /// Create a new Block with the given ID.
     virtual std::shared_ptr<Block> generateBlock(int id) = 0;
 
     /// Return just the block type character ('I','J','L','O','S','T','Z').
     virtual char generateBlock() = 0;
 
     /// Turn randomness on/off for levels that support it.
     virtual void setRandom(bool random) = 0;
 
     /// Set a script file for scripted levels (e.g., Level 0).
     virtual void setScriptFile(const std::string &filename) = 0;
 };
 


