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

#include "m3d.h"
#include "sdl_opengl.h"




/**
 *  Simple camera / view class.
 */
template < typename T >
class Camera {

public:

    m3d::GMatrix4x4< T > rotation;
    m3d::GMatrix4x4< T > translation;


    /**
     *  Camera initialization.
     */
    Camera ();


    /**
     *  Camera destruction.
     */
    ~Camera ();


    /**
     *  Establish camera projection in a viewport.
     */
    void establish ();

};




#endif
