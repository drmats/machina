/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __CAMERA_H_
#define __CAMERA_H_ 1

#include "sdl_opengl.h"
#include <vector>
#include <algorithm>
#include "m3d.h"

namespace machina {




/**
 *  Perspective projection matrix with helpers.
 */
template <typename T>
class PerspectiveProjection {

private:

    using Matrix = m3d::GMatrix4x4<T>;

    /**
     *  Perspective projection matrix.
     */
    Matrix projection;

    /**
     *  Helper vars.
     */
    T fovy = static_cast<T>(60.0f);
    T aspect = static_cast<T>(16.0f / 9.0f);
    T zNear = static_cast<T>(1.0f);
    T zFar = static_cast<T>(8000.0f);


public:

    /**
     *  Load default perspective at construction time.
     */
    PerspectiveProjection () {
        this->projection.load_perspective(
            this->fovy,
            this->aspect,
            this->zNear,
            this->zFar
        );
    }


    /**
     *  Return projection matrix.
     */
    inline const Matrix& get_matrix () const {
        return this->projection;
    }


    /**
     *  Get all parameters as a vector.
     */
    inline std::vector<T> get_all () const {
        std::vector<T> all = {
            this->fovy,
            this->aspect,
            this->zNear,
            this->zFar
        };
        return all;
    }


    /**
     *  Set all parameters from a given vector.
     */
    inline const Matrix& set_all (const std::vector<T> &all) {
        this->fovy = all[0];
        this->aspect = all[1];
        this->zNear = all[2];
        this->zFar = all[3];
        return this->projection.load_perspective(
            this->fovy,
            this->aspect,
            this->zNear,
            this->zFar
        );
    }

};




/**
 *  Simple camera / view class.
 */
template <typename T>
class Camera {

public:

    using Matrix = m3d::GMatrix4x4<T>;


    /**
     *  Perspective projection matrix.
     */
    PerspectiveProjection<T> projection;


    /**
     *  Modelview matrices.
     */
    Matrix rotation;
    Matrix translation;


    /**
     *  dist -- distance to target
     *  pitch -- angle on x-axe
     *  roll -- angle on y-axe
     */
    T dist = 100, pitch = 0, roll = 0;


    /**
     *  Camera initialization.
     */
    Camera ();


    /**
     *  Recompute rotation matrix from dist/pitch/roll.
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
