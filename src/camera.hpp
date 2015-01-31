/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __CAMERA_HPP_
#define __CAMERA_HPP_ 1

#include <vector>
#include "m3d.hpp"
#include "gframe.hpp"

namespace machina {




/**
 *  Perspective projection matrix with helpers.
 */
template <typename T>
class PerspectiveProjection {

    using mat4 = m3d::GMatrix4<T>;


private:

    /**
     *  Perspective projection matrix.
     */
    mat4 projection;

    /**
     *  Helper vars.
     */
    T fovy = static_cast<T>(60.0f);
    T aspect = static_cast<T>(16.0f / 9.0f);
    T z_near = static_cast<T>(0.1f);
    T z_far = static_cast<T>(50000.0f);


public:

    /**
     *  Load default perspective at construction time.
     */
    PerspectiveProjection () {
        this->projection.load_perspective(
            this->fovy,
            this->aspect,
            this->z_near,
            this->z_far
        );
    }


    /**
     *  Return projection matrix.
     */
    inline const mat4& get_matrix () const {
        return this->projection;
    }


    /**
     *  Get all parameters as a vector.
     */
    inline std::vector<T> get_all () const {
        std::vector<T> all = {
            this->fovy,
            this->aspect,
            this->z_near,
            this->z_far
        };
        return all;
    }


    /**
     *  Set all parameters from a given vector.
     */
    inline const mat4& set_all (const std::vector<T> &all) {
        this->fovy = all[0];
        this->aspect = all[1];
        this->z_near = all[2];
        this->z_far = all[3];
        return this->projection.load_perspective(
            this->fovy,
            this->aspect,
            this->z_near,
            this->z_far
        );
    }

};




/**
 *  Simple camera / view class.
 */
template <typename T>
class Camera {

    using vec2 = m3d::GVector2<T>;
    using vec3 = m3d::GVector3<T>;
    using vec4 = m3d::GVector4<T>;
    using mat4 = m3d::GMatrix4<T>;
    using frame = m3d::GFrame<T>;


public:

    /**
     *  Perspective projection matrix.
     */
    PerspectiveProjection<T> projection;


    /**
     *  Camera's frame of reference.
     */
    frame transform;


    /**
     *  Camera target ("look at" point).
     */
    vec3 target;


    /**
     *  dist  -- distance to the target
     *  yaw   -- angle on y-axe
     *  pitch -- angle on x-axe
     */
    T
        dist = static_cast<T>(80),
        yaw = static_cast<T>(330),
        pitch = static_cast<T>(345);


    /**
     *  Camera initialization.
     */
    Camera ();


    /**
     *  Recompute frame of reference from target/dist/pitch/yaw.
     */
    void recompute_transform ();


    /**
     *  Rotate camera using it's local-x axis.
     */
    void relative_rotate_x (T);


    /**
     *  Rotate camera using global-y axis.
     */
    void relative_rotate_y (T);


    /**
     *  Move camera relatively to it's direction on (local-x, local-z) surface.
     */
    void move_on_xz (T, T);


    /**
     *  Move camera relatively to it's direction on (local-x,global-y) surface.
     */
    void move_on_xy (T, T);


    /**
     *  Move camera target along camera's local z axis.
     */
    void forward (T);


    /**
     *  Move camera target along camera's local x axis.
     */
    void strafe (T);


    /**
     *  Move camera target alongs camera's local y axis.
     */
    void up (T);


    /**
     *  Get View-Projection matrix.
     */
    mat4 get_vp_matrix () const;

};




/**
 *  Camera transformer (for smooth translations).
 */
template <typename T>
class CameraTransformer {

protected:

    Camera<T> *camera = nullptr;

    T
        acceleration_factor = 0.35f,
        decceleration_factor = 0.20f,
        max_factor = 0.015,
        factor[6] = {
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f
        };

    bool movement[6] = {
        false, false,
        false, false,
        false, false
    };


public:

    inline void clamp (T &val, T min, T max) {
        if (val < min) { val = min; }
        if (val > max) { val = max; }
    }


    void assign_camera (Camera<T> *camera) {
        this->camera = camera;
    }


    inline void start_moving_forward () { this->movement[0] = true; }
    inline void stop_moving_forward () { this->movement[0] = false; }
    inline void start_moving_backward () { this->movement[1] = true; }
    inline void stop_moving_backward () { this->movement[1] = false; }
    inline void start_moving_left () { this->movement[2] = true; }
    inline void stop_moving_left () { this->movement[2] = false; }
    inline void start_moving_right () { this->movement[3] = true; }
    inline void stop_moving_right () { this->movement[3] = false; }
    inline void start_moving_up () { this->movement[4] = true; }
    inline void stop_moving_up () { this->movement[4] = false; }
    inline void start_moving_down () { this->movement[5] = true; }
    inline void stop_moving_down () { this->movement[5] = false; }


    void update (GLulong elapsed_time, GLulong total_time) {
        for (int i = 0;  i < 6;  i++) {
            if (this->movement[i]) {
                this->clamp(
                    this->factor[i] +=
                        elapsed_time * this->acceleration_factor,
                    0.0f, 100.0f
                );
            } else {
                this->clamp(
                    this->factor[i] -=
                        elapsed_time * decceleration_factor,
                    0.0f, 100.0f
                );
            }
        }

        this->camera->forward(elapsed_time * (
            m3d::linear_interpolation(
                0.0f, this->factor[0], 100.0f,
                0.0f, this->max_factor
            ) - m3d::linear_interpolation(
                0.0f, this->factor[1], 100.0f,
                0.0f, this->max_factor
            ) + std::cos(total_time*0.004f)*0.0004f
        ) * std::sqrt(this->camera->dist));
        this->camera->strafe(elapsed_time * (
            m3d::linear_interpolation(
                0.0f, this->factor[3], 100.0f,
                0.0f, this->max_factor
            ) - m3d::linear_interpolation(
                0.0f, this->factor[2], 100.0f,
                0.0f, this->max_factor
            ) + std::sin(total_time*0.006f)*0.0004f
        ) * std::sqrt(this->camera->dist));
        this->camera->up(elapsed_time * (
            m3d::linear_interpolation(
                0.0f, this->factor[4], 100.0f,
                0.0f, this->max_factor
            ) - m3d::linear_interpolation(
                0.0f, this->factor[5], 100.0f,
                0.0f, this->max_factor
            ) + std::cos(total_time*0.005f)*0.0004f
        ) * std::sqrt(this->camera->dist));
    }

};




} // namespace machina

#endif
