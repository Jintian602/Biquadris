// JBlock module - interface
export module jblock;

import block;

export class JBlock : public Block {
    int rotation = 0;
    
public:
    JBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


