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

    using vec3 = GVector3<T>;
    using vec4 = GVector4<T>;
    using mat4 = GMatrix4<T>;


    /**
     *  Vectors defining this frame.
     */
    vec3
        origin,
        up,
        forward;


    /**
     *  GFrame initialization.
     */
    GFrame ();
    GFrame (const mat4);


    /**
     *  Frame normalization
     *  ("forward" and "up" vectors should be perpendicular and of unit length).
     */
    GFrame<T>& normalize ();


    /**
     *  Translate this frame of reference in the world coordinates.
     */
    inline GFrame<T>& translate_world_x (T delta) {
        this->origin[0] += delta;
        return *this;
    }


    inline GFrame<T>& translate_world_y (T delta) {
        this->origin[1] += delta;
        return *this;
    }


    inline GFrame<T>& translate_world_z (T delta) {
        this->origin[2] += delta;
        return *this;
    }


    inline GFrame<T>& translate_world (T delta_x, T delta_y, T delta_z) {
        this->translate_world_x(delta_x);
        this->translate_world_y(delta_y);
        this->translate_world_z(delta_z);
        return *this;
    }


    inline GFrame<T>& translate_world (const vec3 &v) {
        this->origin.add(v);
        return *this;
    }


    /**
     *  Translate this frame of reference in the local coordinates.
     */
    inline GFrame<T>& translate_local_x (T delta) {
        this->origin.add(
            vec3()
                .cross(this->up, this->forward)
                .scale(delta)
        );
        return *this;
    }


    inline GFrame<T>& translate_local_y (T delta) {
        this->origin.add(this->up * delta);
        return *this;
    }


    inline GFrame<T>& translate_local_z (T delta) {
        this->origin.add(this->forward * delta);
        return *this;
    }


    inline GFrame<T>& translate_local (T delta_x, T delta_y, T delta_z) {
        this->translate_local_x(delta_x);
        this->translate_local_y(delta_y);
        this->translate_local_z(delta_z);
        return *this;
    }


    inline GFrame<T>& translate_local (const vec3 &v) {
        this->translate_local_x(v[0]);
        this->translate_local_y(v[1]);
        this->translate_local_z(v[2]);
        return *this;
    }


    /**
     *  Rotate this frame of reference in the world coordinates.
     */
    inline GFrame<T>& rotate_world (T angle, T x, T y, T z) {
        mat4 rotation(mat4().load_rotation(angle, x, y, z));
        this->up.assign(rotation * vec4(this->up, 0));
        this->forward.assign(rotation * vec4(this->up, 0));
        return *this;
    }


    inline GFrame<T>& rotate_world (T angle, const vec3 &v) {
        return this->rotate_world(angle, v[0], v[1], v[2]);
    }


    inline GFrame<T>& rotate_world_x (T angle) {
        return this->rotate_world (angle, 1, 0, 0);
    }


    inline GFrame<T>& rotate_world_y (T angle) {
        return this->rotate_world (angle, 0, 1, 0);
    }


    inline GFrame<T>& rotate_world_z (T angle) {
        return this->rotate_world (angle, 0, 0, 1);
    }


    /**
     *  Assemble the transformation matrix.
     */
    mat4 get_transformation_matrix () const;


    /**
     *  Assemble the view matrix.
     */
    mat4 get_view_matrix () const;


    /**
     *  Rebuild this GFrame on a given matrix basis.
     */
    GFrame<T>& rebuild_from_matrix (const mat4);

};




} // namespace m3d

#endif
