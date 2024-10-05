#pragma once
#include <game/bases/d_actor.hpp>

class dScoreMng_c {
public:
    void FUN_800e24b0(float, float, dActor_c *, bool);
    void FUN_800e2190(float, float, dActor_c *, bool, short);

    static dScoreMng_c *m_instance;
    static float smc_SCORE_X;
    static float smc_SCORE_Y;
};
