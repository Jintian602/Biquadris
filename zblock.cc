// ZBlock module - interface
export module zblock;

import block;

export class ZBlock : public Block {
    int rotation = 0;
    
public:
    ZBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


