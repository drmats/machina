/**
 *  machina
 *
 *  Copyright (c) 2015, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __GFRAME_CPP_
#define __GFRAME_CPP_ 1

#include "gframe.hpp"

namespace m3d {




/**
 *  GFrame initialization.
 */
template <typename T>
GFrame<T>::GFrame ():
    up { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) },
    forward { static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1) }
    {}


template <typename T>
GFrame<T>::GFrame (const mat4 m) {
    this->rebuild_from_matrix(m);
}




/**
 *  Frame normalization
 *  ("forward" and "up" vectors should be perpendicular and of unit length).
 */
template <typename T>
GFrame<T>& GFrame<T>::normalize () {
    this->forward
        .cross(
            vec3().cross(this->up, this->forward),
            this->up
        )
        .normalize();
    this->up.normalize();
    return *this;
}




/**
 *  Assemble the transformation matrix.
 */
template <typename T>
typename GFrame<T>::mat4 GFrame<T>::get_transformation_matrix () const {
    vec3 right(vec3().cross(this->up, this->forward));
    mat4 m;

    #define set(i, val) m[i] = val
    #define up this->up
    #define forward this->forward
    #define origin this->origin

    set(0, right[0]);  set(4, up[0]);  set( 8, forward[0]);  set(12, origin[0]);
    set(1, right[1]);  set(5, up[1]);  set( 9, forward[1]);  set(13, origin[1]);
    set(2, right[2]);  set(6, up[2]);  set(10, forward[2]);  set(14, origin[2]);
    set(3,     (T)0);  set(7,  (T)0);  set(11,       (T)0);  set(15,      (T)1);

    #undef origin
    #undef forward
    #undef up
    #undef set

    return m;
}




/**
 *  Assemble the view matrix.
 */
template <typename T>
typename GFrame<T>::mat4 GFrame<T>::get_view_matrix () const {
    vec3
        forward(vec3(this->forward).flip()),
        right(vec3().cross(this->up, forward)),
        origin(vec3(this->origin).flip());
    mat4
        rotation,
        translation(mat4().load_translation(origin));

    #define set(i, val) rotation[i] = val
    #define up this->up

    set(0,   right[0]);  set(4,   right[1]);  set( 8,   right[2]);  set(12, (T)0);
    set(1,      up[0]);  set(5,      up[1]);  set( 9,      up[2]);  set(13, (T)0);
    set(2, forward[0]);  set(6, forward[1]);  set(10, forward[2]);  set(14, (T)0);
    set(3,       (T)0);  set(7,       (T)0);  set(11,       (T)0);  set(15, (T)1);

    #undef up
    #undef set

    return rotation * translation;
}




/**
 *  Rebuild this frame of reference on a given matrix basis.
 */
template <typename T>
GFrame<T>& GFrame<T>::rebuild_from_matrix (const mat4 m) {
    this->origin.assign(m * vec4(0, 0, 0, 1));
    this->up.assign(m * vec4(0, 1, 0, 0));
    this->forward.assign(m * vec4(0, 0, -1, 0));
    return *this;
}




/**
 *  Instantiation of allowed types.
 */
template class GFrame<GLfloat>;
template class GFrame<GLdouble>;




} // namespace m3d

#endif
