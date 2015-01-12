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
        this->origin += vec3(delta_x, delta_y, delta_z);
        return *this;
    }


    inline GFrame<T>& translate_world (const vec3 &v) {
        this->origin += v;
        return *this;
    }


    /**
     *  Translate this frame of reference in the local coordinates.
     */
    inline GFrame<T>& translate_local_x (T delta) {
        this->origin +=
            vec3()
                .cross(this->up, this->forward)
                .scale(delta);
        return *this;
    }


    inline GFrame<T>& translate_local_y (T delta) {
        this->origin += this->up * delta;
        return *this;
    }


    inline GFrame<T>& translate_local_z (T delta) {
        this->origin += this->forward * delta;
        return *this;
    }


    inline GFrame<T>& translate_local (T delta_x, T delta_y, T delta_z) {
        this->translate_local_x(delta_x);
        this->translate_local_y(delta_y);
        this->translate_local_z(delta_z);
        return *this;
    }


    inline GFrame<T>& translate_local (const vec3 &v) {
        return this->translate_local(v[0], v[1], v[2]);
    }


    /**
     *  Rotate this frame of reference in the world coordinates.
     */
    inline GFrame<T>& rotate_world (T angle, T x, T y, T z) {
        mat4 rotation(mat4().load_rotation(angle, x, y, z));
        this->up = rotation * vec4(this->up, 0);
        this->forward = rotation * vec4(this->forward, 0);
        return *this;
    }


    inline GFrame<T>& rotate_world (T angle, const vec3 &v) {
        return this->rotate_world(angle, v[0], v[1], v[2]);
    }


    inline GFrame<T>& rotate_world_x (T angle) {
        return this->rotate_world(angle, 1, 0, 0);
    }


    inline GFrame<T>& rotate_world_y (T angle) {
        return this->rotate_world(angle, 0, 1, 0);
    }


    inline GFrame<T>& rotate_world_z (T angle) {
        return this->rotate_world(angle, 0, 0, 1);
    }


    /**
     *  Rotate this frame of reference in the local coordinates.
     */
    inline GFrame<T>& rotate_local_x (T angle) {
        mat4 rotation(mat4().load_rotation(
            angle, vec3().cross(this->up, this->forward)
        ));
        this->up = rotation * vec4(this->up, 0);
        this->forward = rotation * vec4(this->forward, 0);
        return *this;
    }


    inline GFrame<T>& rotate_local_y (T angle) {
        this->forward =
            mat4().load_rotation(angle, this->up) * vec4(this->forward, 0);
        return *this;
    }


    inline GFrame<T>& rotate_local_z (T angle) {
        this->up =
            mat4().load_rotation(angle, this->forward) * vec4(this->up, 0);
        return *this;
    }


    inline GFrame<T>& rotate_local (T angle, const vec3 &v) {
        return this->rotate_world(angle, vec3(
            this->get_transformation_matrix() * vec4(v, 0)
        ));
    }


    inline GFrame<T>& rotate_local (T angle, T x, T y, T z) {
        return this->rotate_local(angle, vec3(x, y, z));
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
     *  Rebuild this frame of reference on a given matrix basis.
     */
    GFrame<T>& rebuild_from_matrix (const mat4);


    /**
     *  Transform this frame of reference by the given matrix.
     */
    GFrame<T>& transform_by_matrix (const mat4);

};


/**
 *  Multiply matrix by a frame of reference yielding the new frame of reference.
 */
template <typename T, std::size_t N = 4>
inline GFrame<T> operator* (const GArray<T, N*N> &m, const GFrame<T> &f) {
    return GFrame<T>(f).transform_by_matrix(m);
}


/**
 *  Multiply a frame of reference by the matrix yielding the new matrix.
 */
template <typename T, std::size_t N = 4>
inline GMatrix<T, N> operator* (const GFrame<T> &f, const GArray<T, N*N> &m) {
    return f.get_transformation_matrix() * m;
}




} // namespace m3d

#endif
