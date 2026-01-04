// OBlock module - interface
export module oblock;

import block;

export class OBlock : public Block {
public:
    OBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


