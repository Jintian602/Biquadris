// LBlock module - interface
export module lblock;

import block;

export class LBlock : public Block {
    int rotation = 0;
    
public:
    LBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


