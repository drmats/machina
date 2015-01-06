/**
 *  machina
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __GFRAME_HPP_
#define __GFRAME_HPP_ 1

#include "sdl_opengl.hpp"
#include "m3d.hpp"

namespace machina {




/**
 *  Frame of reference.
 */
template <typename T>
class GFrame {

public:

    using Matrix = m3d::GMatrix4x4<T>;
    using Vector3 = m3d::GVector3<T>;


    /**
     *  Vectors defining this frame.
     */
    Vector3
        origin,
        forward,
        up;


    /**
     *  GFrame initialization.
     */
    GFrame ();


    /**
     *  Translate this frame of reference in a world coordinates.
     */
    inline void translate_x (T delta) { this->origin[0] += delta; }
    inline void translate_y (T delta) { this->origin[1] += delta; }
    inline void translate_z (T delta) { this->origin[2] += delta; }
    inline void translate_world (T delta_x, T delta_y, T delta_z) {
        this->translate_x(delta_x);
        this->translate_y(delta_y);
        this->translate_z(delta_z);
    }
    inline void translate_world (const Vector3 &v) {
        this->origin.add(v);
    }


    /**
     *  Translate this frame of reference in a local coordinates.
     */
    inline void translate_right (T delta) {
        Vector3 right; right.cross(this->up, this->forward);
        this->origin.add(right.scale(delta));
    }
    inline void translate_up (T delta) {
        Vector3 up(this->up);
        this->origin.add(up.scale(delta));
    }
    inline void translate_forward (T delta) {
        Vector3 forward(this->forward);
        this->origin.add(forward.scale(delta));
    }
    inline void translate_local (T delta_x, T delta_y, T delta_z) {
        this->translate_right(delta_x);
        this->translate_up(delta_y);
        this->translate_forward(delta_z);
    }
    inline void translate_local (const Vector3 &v) {
        this->translate_right(v[0]);
        this->translate_up(v[1]);
        this->translate_forward(v[2]);
    }

};




} // namespace machina

#endif
