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

private:

    using mat4 = m3d::GMatrix4<T>;

    /**
     *  Perspective projection matrix.
     */
    mat4 projection;

    /**
     *  Helper vars.
     */
    T fovy = static_cast<T>(60.0f);
    T aspect = static_cast<T>(16.0f / 9.0f);
    T z_near = static_cast<T>(1.0f);
    T z_far = static_cast<T>(200000.0f);


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

public:

    using mat4 = m3d::GMatrix4<T>;
    using vec3 = m3d::GVector3<T>;


    /**
     *  Perspective projection matrix.
     */
    PerspectiveProjection<T> projection;


    /**
     *  Camera rotation representation.
     */
    mat4 rotation;


    /**
     *  Camera translation representation.
     */
    vec3 translation;


    /**
     *  dist  -- distance to the target
     *  yaw   -- angle on y-axe
     *  pitch -- angle on x-axe
     */
    T
        dist = static_cast<T>(80.0f),
        yaw = static_cast<T>(30.0f),
        pitch = static_cast<T>(15.0f);


    /**
     *  Camera initialization.
     */
    Camera ();


    /**
     *  Recompute rotation matrix from dist/pitch/yaw.
     */
    void recompute_rotation ();


    /**
     *  Rotate camera (look-around).
     */
    void relative_rotate (T, T, T, T);


    /**
     *  Relative translate camera (move).
     */
    void relative_translate (const std::vector<char> &, T, T);
    void relative_translate_on_xz (const std::vector<char> &, T, T);
    static const std::vector<char> strafe;
    static const std::vector<char> up;
    static const std::vector<char> out;


    /**
     *  Establish camera projection in a viewport.
     */
    void establish_projection () const;


    /**
     *  Establish camera modelview in a viewport.
     */
    void establish_modelview () const;

};




} // namespace machina

#endif
