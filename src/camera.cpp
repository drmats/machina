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
 *  Rotate camera using it's local-x axis.
 */
template <typename T>
void Camera<T>::relative_rotate_x (T angle) {
    this->transform.rotate_local_x(angle);
    this->pitch = m3d::positive_fmod(this->pitch - angle, (T)360);
    this->target =
        this->transform.origin + this->transform.forward * this->dist;

}




/**
 *  Rotate camera using global-y axis.
 */
template <typename T>
void Camera<T>::relative_rotate_y (T angle) {
    this->transform.rotate_world_y(angle);
    this->yaw = m3d::positive_fmod(this->yaw + angle, (T)360);
    this->target =
        this->transform.origin + this->transform.forward * this->dist;
}



/**
 *  Move camera relatively to it's direction on (local-x, local-z) surface.
 */
template <typename T>
void Camera<T>::move_on_xz (T strafe, T forward) {
    vec3 right = vec3()
        .cross(this->transform.up, this->transform.forward);
    vec2
        right_flat =
            vec2(right[0], right[2])
                .normalize(),
        forward_flat =
            vec2(this->transform.forward[0], this->transform.forward[2])
                .normalize();

    if (forward_flat.is_zero()) {
        forward_flat =
            vec2(this->transform.up[0], this->transform.up[2])
                .normalize();
    }

    right_flat.scale(strafe);
    forward_flat.scale(forward);

    this->target[0] += right_flat[0] + forward_flat[0];
    this->target[2] += right_flat[1] + forward_flat[1];

    this->recompute_transform();
}




/**
 *  Move camera relatively to it's direction on (local-x,global-y) surface.
 */
template <typename T>
void Camera<T>::move_on_xy (T strafe, T up) {
    vec3 right = vec3()
        .cross(this->transform.up, this->transform.forward);
    vec2 right_flat = vec2(right[0], right[2])
        .normalize();

    right_flat.scale(strafe);

    this->target[0] += right_flat[0];
    this->target[1] += up;
    this->target[2] += right_flat[1];

    this->recompute_transform();
}


/**
 *  Move camera target along camera's local z axis.
 */
template <typename T>
void Camera<T>::forward (T delta) {
    this->target += this->transform.forward * delta;
    this->recompute_transform();
}


/**
 *  Move camera target along camera's local x axis.
 */
template <typename T>
void Camera<T>::strafe (T delta) {
    vec3 right = vec3()
        .cross(this->transform.forward, this->transform.up);
    this->target += right * delta;
    this->recompute_transform();
}


/**
 *  Move camera target alongs camera's local y axis.
 */
template <typename T>
void Camera<T>::up (T delta) {
    this->target += this->transform.up * delta;
    this->recompute_transform();
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
