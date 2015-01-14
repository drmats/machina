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

#include "primitives.hpp"
#include <cstdlib>

namespace machina {
    namespace primitives {




using vec3 = m3d::GVector3<GLfloat>;
using vec4 = m3d::GVector4<GLfloat>;




/**
 *  Axes.
 */
std::shared_ptr<VertexBatch> axes (GLfloat dim, GLfloat s) {
    std::vector<vec3> v;
    std::vector<vec4> c;
    auto batch = std::make_shared<VertexBatch>();
    const GLfloat i = 0.7;

    dim = dim * 0.5;

    #define line(x1, y1, z1, x2, y2, z2, r1, g1, b1, a1, r2, g2, b2, a2) \
        v.push_back(vec3(x1, y1, z1)); c.push_back(vec4(r1, g1, b1, a1)); \
        v.push_back(vec3(x2, y2, z2)); c.push_back(vec4(r2, g2, b2, a2));

    // red axis
    line(-dim-2*s,   0,  0, dim+2*s, 0, 0,   i-0.2, 0, 0, 0.5, i, 0, 0, 1);
    line( dim+2*s-4, 0,  2, dim+2*s, 0, 0,   i    , 0, 0, 1  , i, 0, 0, 1);
    line( dim+2*s-4, 0, -2, dim+2*s, 0, 0,   i    , 0, 0, 1  , i, 0, 0, 1);

    // green axis
    line( 0, -s*2,   0, 0, s*2, 0,   0, i-0.2, 0, 0.5, 0, i, 0, 1);
    line( 2,  s*2-4, 0, 0, s*2, 0,   0, i    , 0, 1  , 0, i, 0, 1);
    line(-2,  s*2-4, 0, 0, s*2, 0,   0, i    , 0, 1  , 0, i, 0, 1);

    // blue axis
    line( 0, 0, -dim-2*s,   0, 0, dim+2*s,   0, 0, i-0.2, 0.5, 0, 0, i, 1);
    line( 2, 0,  dim+2*s-4, 0, 0, dim+2*s,   0, 0, i    , 1  , 0, 0, i, 1);
    line(-2, 0,  dim+2*s-4, 0, 0, dim+2*s,   0, 0, i    , 1  , 0, 0, i, 1);

    batch->prepare(GL_LINES, v, c);

    #undef line

    return batch;
}




/**
 *  Grid.
 */
std::shared_ptr<VertexBatch> grid (GLfloat dim, GLfloat s, vec4 color) {
    std::vector<vec3> v;
    std::vector<vec4> c;
    auto batch = std::make_shared<VertexBatch>();

    dim = dim * 0.5;

    #define line(x1, y1, z1, x2, y2, z2) \
        v.push_back(vec3(x1, y1, z1)); c.push_back(color); \
        v.push_back(vec3(x2, y2, z2)); c.push_back(color);

    for (GLfloat d = s;  d <= dim;  d += s) {
        line(-dim, 0,  d,    dim, 0,    d);
        line(-dim, 0, -d,    dim, 0,   -d);
        line( d,   0, -dim,  d,   0,  dim);
        line(-d,   0, -dim, -d,   0,  dim);
    }

    #undef line

    batch->prepare(GL_LINES, v, c);

    return batch;
}




/**
 *  Point-cube.
 */
std::shared_ptr<VertexBatch> point_cube (GLfloat dim, GLfloat s, GLfloat a) {
    std::vector<vec3> v;
    std::vector<vec4> c;
    auto batch = std::make_shared<VertexBatch>();
    const GLfloat cstep = (1.0/(2.0f*dim));
    const GLfloat cshift = 0.5f;

    dim = dim * 0.5f;

    for (GLfloat x = -dim; x <= dim; x += s) {
        for (GLfloat y = -dim; y <= dim; y += s) {
            for (GLfloat z = -dim; z <= dim; z += s) {
                v.push_back(vec3(
                    x + (std::rand() % static_cast<int>(s)) - s/2.0f,
                    y + (std::rand() % static_cast<int>(s)) - s/2.0f,
                    z + (std::rand() % static_cast<int>(s)) - s/2.0f
                ));
                c.push_back(vec4(
                    cstep*x+cshift,
                    cstep*y+cshift,
                    cstep*z+cshift,
                    a
                ));
            }
        }
    }

    batch->prepare(GL_POINTS, v, c);

    return batch;
}




/**
 *  This thing...
 */
std::shared_ptr<VertexBatch> this_thing (
    GLfloat dim, GLfloat space,
    GLfloat cfull, GLfloat a, GLenum what
) {
    std::vector<vec3> verts;
    std::vector<vec4> colors;
    auto batch = std::make_shared<VertexBatch>();
    const GLfloat cstep = (cfull/(2.0f*dim));
    const GLfloat cshift = cfull/2.0f;

    dim = dim * 0.5f;

    #define v(x, y, z) verts.push_back(vec3(x, y, z))
    #define c(r, g, b, a) colors.push_back(vec4(r, g, b, a))

    for (
        GLfloat d = -dim;
        d <= dim;
        d += space
    ) {
        v(-1*dim,  1*d, 1*d); c( 0.0f, cstep*d+cshift, cstep*d+cshift, a);
        v( 1*dim, -1*d, 1*d); c(cfull, cshift-cstep*d, cstep*d+cshift, a);
        v(1*d,  1*d, -1*dim); c(cstep*d+cshift, cstep*d+cshift,  0.0f, a);
        v(1*d, -1*d,  1*dim); c(cstep*d+cshift, cshift-cstep*d, cfull, a);

        v(-1*dim, -1*d, 1*d); c( 0.0f, cshift-cstep*d, cstep*d+cshift, a);
        v( 1*dim,  1*d, 1*d); c(cfull, cstep*d+cshift, cstep*d+cshift, a);
        v(1*d, -1*d, -1*dim); c(cstep*d+cshift, cshift-cstep*d,  0.0f, a);
        v(1*d,  1*d,  1*dim); c(cstep*d+cshift, cstep*d+cshift, cfull, a);

        v( 1*d, -1*dim, 1*d); c(cstep*d+cshift,  0.0f, cstep*d+cshift, a);
        v(-1*d,  1*dim, 1*d); c(cshift-cstep*d, cfull, cstep*d+cshift, a);
        v(1*d,  1*d, -1*dim); c(cstep*d+cshift, cstep*d+cshift,  0.0f, a);
        v(-1*d, 1*d,  1*dim); c(cshift-cstep*d, cstep*d+cshift, cfull, a);

        v(-1*d, -1*dim, 1*d); c(cshift-cstep*d,  0.0f, cstep*d+cshift, a);
        v( 1*d,  1*dim, 1*d); c(cstep*d+cshift, cfull, cstep*d+cshift, a);
        v(-1*d, 1*d, -1*dim); c(cshift-cstep*d, cstep*d+cshift,  0.0f, a);
        v( 1*d, 1*d,  1*dim); c(cstep*d+cshift, cstep*d+cshift, cfull, a);
    }

    #undef c
    #undef v

    batch->prepare(what, verts, colors);

    return batch;
}




    } // namespace primitives
} // namespace grid

#endif
