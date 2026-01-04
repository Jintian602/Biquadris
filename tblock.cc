// TBlock module - interface
export module tblock;

import block;

export class TBlock : public Block {
    int rotation = 0;
    
public:
    TBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


