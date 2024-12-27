#pragma once
#include <game/bases/d_base_actor.hpp>

class daPyMng_c {
public:
    enum PyType {}; ///< @unofficial
    static int getPlayerIndex(PyType); ///< @unofficial
    static dBaseActor_c *getPlayer(int);

    static int mPlayerType[4];
    static int mPlayerMode[4];
    static int mRest[4];
};
