#pragma once
#include <lib/rvl/gx/GX.h>

class mVideo {
public:
    GXRenderModeObj &obj;

    static float getSmth(float offs) { return (m_video->obj.smth2 - offs) * 0.5f; }

    static mVideo *m_video;
};
