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

namespace machina {




/**
 *  Camera initialization.
 */
template <typename T>
Camera<T>::Camera () {
    this->rotation.load_identity();
    this->translation.load_identity();
    this->recompute_rotation();
}




/**
 *  Recompute rotation matrix from dist/pitch/yaw.
 */
template <typename T>
void Camera<T>::recompute_rotation () {
    Matrix op, tmp;
    this->rotation.load_translation(0.0, 0.0, -this->dist);
    this->rotation = op.multiply(
        this->rotation,
        tmp.load_rotation(this->pitch, 1.0, 0.0, 0.0)
    );
    this->rotation = op.multiply(
        this->rotation,
        tmp.load_rotation(this->yaw, 0.0, 1.0, 0.0)
    );
}




/**
 *  Rotate camera (look-around).
 */
template <typename T>
void Camera<T>::relative_rotate (T delta, T x, T y, T z) {
    Matrix op, relative_rotation;
    this->rotation = op.multiply(
        relative_rotation.load_rotation(delta, x, y, z),
        this->rotation
    );
}




/**
 *  Relative translate camera (move).
 */
template <typename T>
void Camera<T>::relative_translate (
    const std::vector<char> &direction,
    T heading,
    T delta
) {
    char i = 12;
    std::for_each(
        direction.begin(),
        direction.end(),
        [this, heading, delta, &i] (char el) {
            this->translation[i] += delta*this->rotation[el]*heading;
            i++;
        }
    );
}

template <typename T>
const std::vector<char> Camera<T>::strafe = { 0, 4, 8 };

template <typename T>
const std::vector<char> Camera<T>::up = { 1, 5, 9 };

template <typename T>
const std::vector<char> Camera<T>::out = { 2, 6, 10 };




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
    Matrix op;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(*op.multiply(this->rotation, this->translation));
}




/**
 *  Establish camera modelview in a viewport (GLdouble specialization).
 */
template <>
void Camera<GLdouble>::establish_modelview () const {
    Matrix op;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(*op.multiply(this->rotation, this->translation));
}




/**
 *  Instantiation for allowed types.
 */
template class Camera<GLfloat>;
template class Camera<GLdouble>;




} // namespace machina

#endif
