/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __PRIMITIVES_CPP_
#define __PRIMITIVES_CPP_ 1

#include "primitives.h"

namespace machina {
    namespace primitives {




    /**
     *  Draw a grid.
     */
    void grid (GLfloat dim, GLfloat space) {
        dim = dim * 0.5f;
        
        #define intensity 0.6f
        #define line(x1, y1, z1, x2, y2, z2) \
            glVertex3f(x1, y1, z1); \
            glVertex3f(x2, y2, z2)

        glPushAttrib(
            GL_CURRENT_BIT |
            GL_ENABLE_BIT |
            GL_LIGHTING_BIT
        );
        glDisable(GL_LIGHTING);
        glColor3f(intensity, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        line(-dim-2.0f*space,      0.0f,  0.0f,    dim+2.0f*space, 0.0f, 0.0f);
        line( dim+2.0f*space-4.0f, 0.0f,  2.0f,    dim+2.0f*space, 0.0f, 0.0f);
        line( dim+2.0f*space-4.0f, 0.0f, -2.0f,    dim+2.0f*space, 0.0f, 0.0f);

        glColor3f(0.0f, intensity, 0.0f);
        line( 0.0f, -space*2.0f,       0.0f,    0.0f, space*2.0f, 0.0f);
        line( 2.0f,  space*2.0f-4.0f,  0.0f,    0.0f, space*2.0f, 0.0f);
        line(-2.0f,  space*2.0f-4.0f,  0.0f,    0.0f, space*2.0f, 0.0f);
        
        glColor3f(0.0f, 0.0f, intensity);
        line( 0.0f, 0.0f, -dim-2.0f*space,         0.0f, 0.0f, dim+2.0f*space);
        line( 2.0f, 0.0f,  dim+2.0f*space-4.0f,    0.0f, 0.0f, dim+2.0f*space);
        line(-2.0f, 0.0f,  dim+2.0f*space-4.0f,    0.0f, 0.0f, dim+2.0f*space);
        glEnd();

        glColor3f(0.2f, 0.2f, 0.3f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (
            GLfloat d = space;
            d <= dim;
            d += space
        ) {
            line(-dim, 0.0f,  d,       dim, 0.0f,  d);
            line(-dim, 0.0f, -d,       dim, 0.0f, -d);
            line( d,   0.0f, -dim,     d,   0.0f,  dim);
            line(-d,   0.0f, -dim,    -d,   0.0f,  dim);
        }
        glEnd();
        glPopAttrib();

        #undef line
        #undef intensity
    }




    } // namespace primitives
} // namespace grid

#endif
