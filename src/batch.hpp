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
 *  Simple batch (just vertices with colors).
 */
class VertexBatch : public Batch {

    using vec3 = m3d::GVector3<GLfloat>;
    using vec4 = m3d::GVector4<GLfloat>;


protected:

    /**
     *  One of GL_POINTS or GL_LINES.
     */
    GLenum draw_mode;


    /**
     *  ...
     */
    GLuint
        vertex_array_object,
        vertex_buffer,
        color_buffer,
        num_verts;


public:

    /**
     *  VertexBatch initialization.
     */
    VertexBatch ();


    /**
     *  Clean-up.
     */
    virtual ~VertexBatch ();


    /**
     *  ...
     */
    VertexBatch& prepare (
        GLenum,
        const std::vector<vec3> &,
        const std::vector<vec4> &
    );


    /**
     *  Draw VertexBatch contents.
     */
    virtual void draw () const;

};




} // namespace machina

#endif
