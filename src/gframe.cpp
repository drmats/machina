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
    forward { static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1) },
    up { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) }
    {}




/**
 *  Frame normalization
 *  ("forward" and "up" vectors should be perpendicular and of unit length).
 */
template <typename T>
void GFrame<T>::normalize () {
    vec3 right;

    this->forward.cross(
        right.cross(this->up, this->forward),
        this->up
    );
    this->up.normalize();
    this->forward.normalize();
}




/**
 *  Assemble the transformation matrix.
 */
template <typename T>
m3d::GMatrix4<T> GFrame<T>::get_transformation_matrix () const {
    mat4 m;
    vec3 right; right.cross(this->up, this->forward);

    #define set(i, val) m[i] = val
    #define up this->up
    #define forward this->forward
    #define origin this->origin
    #define zero static_cast<T>(0)
    #define one static_cast<T>(1)

    set(0, right[0]);  set(4, up[0]);  set( 8, forward[0]);  set(12, origin[0]);
    set(1, right[1]);  set(5, up[1]);  set( 9, forward[1]);  set(13, origin[1]);
    set(2, right[2]);  set(6, up[2]);  set(10, forward[2]);  set(14, origin[2]);
    set(3,     zero);  set(7,  zero);  set(11,       zero);  set(15,       one);

    #undef one
    #undef zero
    #undef origin
    #undef forward
    #undef up
    #undef set

    return m;
}




/**
 *  Instantiation of allowed types.
 */
template class GFrame<GLfloat>;
template class GFrame<GLdouble>;




} // namespace m3d

#endif
