#pragma once

class Remocon {
public:
    virtual ~Remocon();

    enum EXTENSION_TYPE_e {
        EXTENTION_NONE
    };

    u8 pad[4];
    EXTENSION_TYPE_e mAttachedExtension;
};

class dGameKey_c {
public:
    virtual ~dGameKey_c();

    Remocon *mRemocon[4];

    static dGameKey_c *m_instance;
};
