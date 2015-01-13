/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __BATCH_HPP_
#define __BATCH_HPP_ 1

#include "sdl_opengl.hpp"
#include "m3d.hpp"
#include <vector>

namespace machina {




/**
 *  Base class for VBO operations.
 */
class Batch {

public:

    /**
     *  Draw batch contents.
     */
    virtual void draw () const = 0;

};




/**
 *  Simple batch (just vertices).
 */
class SimpleBatch : public Batch {

    using vec3 = m3d::GVector3<GLfloat>;
    using vec4 = m3d::GVector4<GLfloat>;


protected:

    /**
     *  One of GL_POINTS, GL_LINES, ...
     */
    GLenum draw_mode;


    /**
     *  ...
     */
    GLuint
        vertex_array,
        color_array,
        num_verts;


public:

    /**
     *  SimpleBatch initialization.
     */
    SimpleBatch ();


    /**
     *  Clean-up.
     */
    virtual ~SimpleBatch ();


    /**
     *  ...
     */
    SimpleBatch& prepare (GLenum, const std::vector<vec3> &, const std::vector<vec4> &);


    /**
     *  Draw SimpleBatch contents.
     */
    virtual void draw () const;

};




} // namespace machina

#endif
