#pragma once
#include <types.h>
#include <game/mLib/m_vec.hpp>

struct AreaBoundU16 {
    AreaBoundU16(u16 x, u16 y, u16 w, u16 h) {
        this->x = x;
        this->y = y;
        width = w;
        height = h;
    }

    u16 x, y, width, height;
};

class dCdArea_c {
public:
    AreaBoundU16 bound;
};

class dCdUnk_c {
public:
    char pad[8];
    u16 unk;
};

class dCdFile_c {
public:
    char pad[0xc];
    dCdUnk_c *mpUnk;
    char pad2[0x1c];
    dCdArea_c *areas;
    char pad3[0x380];

    u8 getAreaNo(mVec3_c *);
    dCdArea_c *getAreaP(u8 zoneID, AreaBound *bound);
};

class dCd_c {
public:
    dCdFile_c courses[4];

    dCdFile_c *getFileP(int i) {
        dCdFile_c *course = &courses[i];
        if (course->areas != nullptr) {
            return course;
        }
        return nullptr;
    }

    static dCd_c *m_instance;
};
