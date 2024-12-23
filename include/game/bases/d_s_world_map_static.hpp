#pragma once
#include <types.h>

class dScWMap_c {
public:
    enum CourseType_e {
        COURSE_TYPE_2 = 2,
        COURSE_TYPE_80 = 0x80,
        COURSE_TYPE_200 = 0x200
    };

    // static u8 getWorldNo() { return m_WorldNo; }
    static u8 getWorldNo();

    static bool IsCourseType(int, int, CourseType_e);

    static u8 m_WorldNo;
};
