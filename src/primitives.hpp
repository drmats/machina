/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __PRIMITIVES_HPP_
#define __PRIMITIVES_HPP_ 1

#include "sdl_opengl.hpp"

namespace machina {
    namespace primitives {




/**
 *  Draw an axes-grid.
 */
void axes_grid (GLfloat dim, GLfloat space);


/**
 *  Draw a grid.
 */
void grid (GLfloat dim, GLfloat space);


/**
 *  Draw a point-cube.
 */
void point_cube (GLfloat dim, GLfloat space, GLfloat a);


/**
 *  Draw this thing...
 */
void this_thing (
    GLfloat dim, GLfloat space,
    GLfloat cfull, GLfloat psize, GLfloat a, Uint8 what
);




    } // namespace primitives
} // namespace grid

#endif
