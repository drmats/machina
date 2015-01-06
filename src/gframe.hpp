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

#include "m3d.hpp"

namespace m3d {




/**
 *  Frame of reference.
 */
template <typename T>
class GFrame {

public:

    using vec3 = m3d::GVector3<T>;
    using mat4 = m3d::GMatrix4<T>;


    /**
     *  Vectors defining this frame.
     */
    vec3
        origin,
        forward,
        up;


    /**
     *  GFrame initialization.
     */
    GFrame ();


    /**
     *  Frame normalization
     *  ("forward" and "up" vectors should be perpendicular and of unit length).
     */
    void normalize ();


    /**
     *  Translate this frame of reference in the world coordinates.
     */
    inline void translate_x (T delta) { this->origin[0] += delta; }
    inline void translate_y (T delta) { this->origin[1] += delta; }
    inline void translate_z (T delta) { this->origin[2] += delta; }
    inline void translate_world (T delta_x, T delta_y, T delta_z) {
        this->translate_x(delta_x);
        this->translate_y(delta_y);
        this->translate_z(delta_z);
    }
    inline void translate_world (const vec3 &v) {
        this->origin.add(v);
    }


    /**
     *  Translate this frame of reference in the local coordinates.
     */
    inline void translate_right (T delta) {
        vec3 right;
        this->origin.add(
            right
                .cross(this->up, this->forward)
                .scale(delta)
        );
    }
    inline void translate_up (T delta) {
        vec3 up(this->up);
        this->origin.add(up.scale(delta));
    }
    inline void translate_forward (T delta) {
        vec3 forward(this->forward);
        this->origin.add(forward.scale(delta));
    }
    inline void translate_local (T delta_x, T delta_y, T delta_z) {
        this->translate_right(delta_x);
        this->translate_up(delta_y);
        this->translate_forward(delta_z);
    }
    inline void translate_local (const vec3 &v) {
        this->translate_right(v[0]);
        this->translate_up(v[1]);
        this->translate_forward(v[2]);
    }


    /**
     *  Assemble the transformation matrix.
     */
     m3d::GMatrix4<T> get_transformation_matrix () const;


    /**
     *  Assemble the view matrix.
     */
     m3d::GMatrix4<T> get_view_matrix () const;

};




} // namespace m3d

#endif
