#pragma once
#include <game/bases/d_cc.hpp>

class dEn_c;

class FumiCcInfo_c {
public:
    FumiCcInfo_c(dCc_c *cc1, dCc_c *cc2) : mCc1(cc1), mCc2(cc2) {}
    virtual ~FumiCcInfo_c() {}

    dCc_c *mCc1;
    dCc_c *mCc2;
};

/// @unofficial
class dEnFumiNest_c {
public:
    dEnFumiNest_c() {}
    virtual ~dEnFumiNest_c() {}
};

class dEnFumiCheck_c {
public:
    dEnFumiCheck_c(dEn_c *owner) {
        mUnk = 0;
        mNest = nullptr;
        mOwner = owner;
        dEnFumiNest_c *nest = new dEnFumiNest_c();
        delete mNest;
        mNest = nest;
    }
    virtual ~dEnFumiCheck_c() {
        delete mNest;
    }

    int mUnk;
    dEnFumiNest_c *mNest;
    dEn_c *mOwner;
};

class dEnFumiProc_c {
public:
    dEnFumiProc_c(dEn_c *owner) : mFumiCheck(owner) {}

    int operate(FumiCcInfo_c &, int);

    dEnFumiCheck_c mFumiCheck;
};
