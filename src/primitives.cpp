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
        const GLfloat intensity = 0.55f;

        dim = dim * 0.5f;

        #define line(x1, y1, z1, x2, y2, z2) \
            glVertex3f(x1, y1, z1); \
            glVertex3f(x2, y2, z2)

        glPushAttrib(
            GL_CURRENT_BIT |
            GL_ENABLE_BIT |
            GL_LIGHTING_BIT
        );

        glDisable(GL_LIGHTING);
        glLineWidth(2.2f);

        glBegin(GL_LINES);

        glColor3f(intensity, 0.0f, 0.0f);
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

        glLineWidth(1.4f);

        glBegin(GL_LINES);

        glColor3f(0.15f, 0.15f, 0.25f);
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
    }




    /**
     *  Draw this thing...
     */
    void this_thing (
        GLfloat dim, GLfloat space,
        GLfloat cfull, GLfloat psize, GLfloat a, Uint8 what
    ) {
        const GLfloat cstep = (cfull/(2.0f*dim));
        const GLfloat cshift = cfull/2.0f;

        dim = dim * 0.5f;

        glPushAttrib(
            GL_CURRENT_BIT |
            GL_ENABLE_BIT |
            GL_LIGHTING_BIT
        );

        glDisable(GL_LIGHTING);
        glPointSize(psize);
        glLineWidth(psize);

        glBegin(what);
        for (
            GLfloat d = -dim;
            d <= dim;
            d += space
        ) {
            glColor4f( 0.0f, cstep*d+cshift, cstep*d+cshift, a); glVertex3f(-1*dim,  1*d, 1*d);
            glColor4f(cfull, cshift-cstep*d, cstep*d+cshift, a); glVertex3f( 1*dim, -1*d, 1*d);
            glColor4f(cstep*d+cshift, cstep*d+cshift,  0.0f, a); glVertex3f(1*d,  1*d, -1*dim);
            glColor4f(cstep*d+cshift, cshift-cstep*d, cfull, a); glVertex3f(1*d, -1*d,  1*dim);

            glColor4f( 0.0f, cshift-cstep*d, cstep*d+cshift, a); glVertex3f(-1*dim, -1*d, 1*d);
            glColor4f(cfull, cstep*d+cshift, cstep*d+cshift, a); glVertex3f( 1*dim,  1*d, 1*d);
            glColor4f(cstep*d+cshift, cshift-cstep*d,  0.0f, a); glVertex3f(1*d, -1*d, -1*dim);
            glColor4f(cstep*d+cshift, cstep*d+cshift, cfull, a); glVertex3f(1*d,  1*d,  1*dim);

            glColor4f(cstep*d+cshift,  0.0f, cstep*d+cshift, a); glVertex3f( 1*d, -1*dim, 1*d);
            glColor4f(cshift-cstep*d, cfull, cstep*d+cshift, a); glVertex3f(-1*d,  1*dim, 1*d);
            glColor4f(cstep*d+cshift, cstep*d+cshift,  0.0f, a); glVertex3f(1*d,  1*d, -1*dim);
            glColor4f(cshift-cstep*d, cstep*d+cshift, cfull, a); glVertex3f(-1*d, 1*d,  1*dim);

            glColor4f(cshift-cstep*d,  0.0f, cstep*d+cshift, a); glVertex3f(-1*d, -1*dim, 1*d);
            glColor4f(cstep*d+cshift, cfull, cstep*d+cshift, a); glVertex3f( 1*d,  1*dim, 1*d);
            glColor4f(cshift-cstep*d, cstep*d+cshift,  0.0f, a); glVertex3f(-1*d, 1*d, -1*dim);
            glColor4f(cstep*d+cshift, cstep*d+cshift, cfull, a); glVertex3f( 1*d, 1*d,  1*dim);

        }
        glEnd();

        glPopAttrib();
    }




    } // namespace primitives
} // namespace grid

#endif
