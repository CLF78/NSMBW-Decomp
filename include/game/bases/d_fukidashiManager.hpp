#pragma once

struct dfukidashiManager_c_substruct {
    bool smth;
    char pad[0x23b];
};

class dfukidashiManager_c {
public:
    char pad[0x381];
    dfukidashiManager_c_substruct smth[4];

    static dfukidashiManager_c *m_instance;
};
