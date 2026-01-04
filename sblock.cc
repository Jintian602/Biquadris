// SBlock module - interface
export module sblock;

import block;

export class SBlock : public Block {
    int rotation = 0;
    
public:
    SBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


