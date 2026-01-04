// IBlock module - interface
export module iblock;

import block;
import position;
import <vector>;

export class IBlock : public Block {
    int rotation = 0;
    
public:
    IBlock(int id, int level);
    void rotateCW() override;
    void rotateCCW() override;
};


