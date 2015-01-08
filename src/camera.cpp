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
    this->recompute_transform();
}




/**
 *  Recompute rotation matrix from dist/pitch/yaw.
 */
template <typename T>
void Camera<T>::recompute_transform () {
    this->transform.rebuild_from_matrix(
        mat4().load_translation(this->target) *
        mat4().load_rotation(this->yaw, 0, 1, 0) *
        mat4().load_rotation(this->pitch, 1, 0, 0) *
        mat4().load_translation(0, 0, this->dist)
    );
}




/**
 *  Recompute yaw/pitch/target from frame of reference and dist.
 */
template <typename T>
void Camera<T>::recompute_ypt () {
    using vec2 = m3d::GVector2<T>;

    this->target =
        this->transform.origin + this->transform.forward * this->dist;

    #define forward this->transform.forward

    if (forward[0] > 0) {
        this->yaw = m3d::rad_to_deg(std::acos(
            forward[2] / vec2(forward[0], forward[2]).length()
        ) - m3d_pi);
    } else {
        this->yaw = m3d::rad_to_deg(m3d_pi - std::acos(
            forward[2] / vec2(forward[0], forward[2]).length()
        ));
    }

    this->pitch = m3d::rad_to_deg(std::acos(
        forward[2] / vec2(forward[1], forward[2]).length()
    ) + m3d_pi);

    #undef forward
}




/**
 *  Rotate camera (look-around).
 */
template <typename T>
void Camera<T>::relative_rotate_x (T angle) {
    this->transform.rotate_local_x(angle);
    this->recompute_ypt();
}
template <typename T>
void Camera<T>::relative_rotate_y (T angle) {
    this->transform.rotate_world_y(angle);
    this->recompute_ypt();
}




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
