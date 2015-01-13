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
 *  Rotate camera (look-around).
 */
template <typename T>
void Camera<T>::relative_rotate_x (T angle) {
    this->transform.rotate_local_x(angle);
    this->pitch = m3d::positive_fmod(this->pitch - angle, (T)360);
    this->target =
        this->transform.origin + this->transform.forward * this->dist;

}
template <typename T>
void Camera<T>::relative_rotate_y (T angle) {
    this->transform.rotate_world_y(angle);
    this->yaw = m3d::positive_fmod(this->yaw + angle, (T)360);
    this->target =
        this->transform.origin + this->transform.forward * this->dist;
}




/**
 *  Get View-Projection matrix.
 */
template <typename T>
typename Camera<T>::mat4 Camera<T>::get_vp_matrix () const {
    return this->projection.get_matrix() * this->transform.get_view_matrix();
}




/**
 *  Instantiation for allowed types.
 */
template class Camera<GLfloat>;
template class Camera<GLdouble>;




} // namespace machina

#endif
