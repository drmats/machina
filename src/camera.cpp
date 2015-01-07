/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __CAMERA_CPP_
#define __CAMERA_CPP_ 1

#include "camera.hpp"
#include <iostream>
namespace machina {




/**
 *  Camera initialization.
 */
template <typename T>
Camera<T>::Camera () {
    this->recompute_transform();
}




/**
 *  Recompute rotation matrix from dist/pitch/yaw.
 */
template <typename T>
void Camera<T>::recompute_transform () {
    mat4 transform_matrix(
        mat4().load_translation(this->target) *
        mat4().load_rotation(this->yaw, 0, 1, 0) *
        mat4().load_rotation(this->pitch, 1, 0, 0) *
        mat4().load_translation(0, 0, this->dist)
    );


    // mat4 tm, m2, m3;
    // tm.multiply(
    //     m2.load_translation(this->target),
    //     m3.multiply(
    //         tm.load_rotation(this->yaw, 0.0, 1.0, 0.0),
    //         m2.multiply(
    //             m3.load_rotation(this->pitch, 1.0, 0.0, 0.0),
    //             tm.load_translation(0.0, 0.0, this->dist)
    //         )
    //     )
    // );

    // m3d::GVector4<T> u, v;
    // u.transform(tm, v.assign(0, 1, 0, 0));
    // this->transform.up[0] = u[0];
    // this->transform.up[1] = u[1];
    // this->transform.up[2] = u[2];
    // u.transform(tm, v.assign(0, 0, -1, 0));
    // this->transform.forward[0] = u[0];
    // this->transform.forward[1] = u[1];
    // this->transform.forward[2] = u[2];
    // u.transform(tm, v.assign(0, 0, 0, 1));
    // this->transform.origin[0] = u[0];
    // this->transform.origin[1] = u[1];
    // this->transform.origin[2] = u[2];

    this->transform.up[0] = transform_matrix[4];
    this->transform.up[1] = transform_matrix[5];
    this->transform.up[2] = transform_matrix[6];
    this->transform.forward[0] = -transform_matrix[8];
    this->transform.forward[1] = -transform_matrix[9];
    this->transform.forward[2] = -transform_matrix[10];
    this->transform.origin[0] = transform_matrix[12];
    this->transform.origin[1] = transform_matrix[13];
    this->transform.origin[2] = transform_matrix[14];
}




/**
 *  Rotate camera (look-around).
 */
// template <typename T>
// void Camera<T>::relative_rotate (T delta, T x, T y, T z) {
//     mat4 op, relative_rotation;
//     this->rotation = op.multiply(
//         relative_rotation.load_rotation(delta, x, y, z),
//         this->rotation
//     );
// }




/**
 *  Establish camera projection in a viewport (GLfloat specialization).
 */
template <>
void Camera<GLfloat>::establish_projection () const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*this->projection.get_matrix());
}




/**
 *  Establish camera projection in a viewport (GLdouble specialization).
 */
template <>
void Camera<GLdouble>::establish_projection () const {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(*this->projection.get_matrix());
}




/**
 *  Establish camera modelview in a viewport (GLfloat specialization).
 */
template <>
void Camera<GLfloat>::establish_modelview () const {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*this->transform.get_view_matrix());
}




/**
 *  Establish camera modelview in a viewport (GLdouble specialization).
 */
template <>
void Camera<GLdouble>::establish_modelview () const {
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(*this->transform.get_view_matrix());
}




/**
 *  Instantiation for allowed types.
 */
template class Camera<GLfloat>;
template class Camera<GLdouble>;




} // namespace machina

#endif
