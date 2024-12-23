#pragma once
#include <lib/rvl/gx/GX.h>

class mVideo {
public:
    GXRenderModeObj &obj;

    static float getSmth(float offs) { return obj.smth - offs; }

    static mVideo *m_video;
};
