#pragma once

namespace nw4r {
namespace math {

/// @brief A two-dimensional floating point vector.
struct _VEC2 {
    float x, y;
};

struct VEC2 : public _VEC2 {
    VEC2() {}
    VEC2(float fx, float fy) { x = fx; y = fy; }
    VEC2(const VEC2 &v) { x = v.x; y = v.y; }
};

struct _VEC3 {
    float x, y, z;
};

/// @brief A three-dimensional floating point vector.
struct VEC3 : public _VEC3 {
    VEC3() {}
    VEC3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

    void set(const VEC3 &v) { x = v.x; y = v.y; z = v.z; }
    void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

    void reset() { set(0, 0, 0); }
};

} // namespace math
} // namespace nw4r
