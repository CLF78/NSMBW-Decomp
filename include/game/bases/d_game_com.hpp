#pragma once
#include <types.h>
#include <lib/nw4r/lyt/lyt_picture.hpp>
#include <game/mLib/m_3d.hpp>
#include <game/mLib/m_vec.hpp>

/// @brief A collection of various functions used throughout the game.
/// @details Name stands for Game Common.
/// @ingroup bases
namespace dGameCom {
    u32 getRandomSeed(); ///< Gets a seed used for randomness.
    void clearGameStop();
    float getDispCenterY();

    bool isGameStop(unsigned long);
    void SetSoftLight_Player(m3d::bmdl_c&, int);
    void SetSoftLight_Map(m3d::bmdl_c&, int);
    void SetSoftLight_Boss(m3d::bmdl_c&, int);
    void SetSoftLight_Enemy(m3d::bmdl_c&, int);
    void SetSoftLight_MapObj(m3d::bmdl_c&, int);
    void SetSoftLight_Item(m3d::bmdl_c&, int);

    bool someCheck(mVec3_c *a, AreaBound *b);
    void CreateSmallScore(const mVec3_c &, int, int, bool);
    u8 GetAspectRatio();
    bool PlayerEnterCheck(int);

    /// @unofficial
    void updateSelectCursor(nw4r::lyt::Picture *pic, int index, bool useSpecialDraw);

    void DispSizeScale(nw4r::math::VEC2 &scale);
}
