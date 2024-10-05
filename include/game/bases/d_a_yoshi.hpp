#pragma once
#include <game/bases/d_a_player_base.hpp>

class daYoshi_c : public daPlBase_c {
public:
    u8 pad2[0xA0];
    int _a0;

    void getMouthMtx(mMtx_c *);
    void getTongueTipMtx(mMtx_c *);
};