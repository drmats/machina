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

#include "m3d.hpp"
#include <vector>

namespace machina {




/**
 *  Base class for VBO operations.
 */
class Batch {

public:

    /**
     *  Generic buffer index names.
     */
    enum buf_index : GLushort {
        verts = 0,
        normals = 1,
        uvs = 2,
        indices = 3
    };


    /**
     *  Draw batch contents.
     */
    virtual void draw () const = 0;

};




/**
 *  Simple batch (just vertices with colors).
 */
class VertexColorBatch : public Batch {

    using vec3 = m3d::GVector3<GLfloat>;
    using vec4 = m3d::GVector4<GLfloat>;


protected:

    /**
     *  One of GL_POINTS or GL_LINES.
     */
    GLenum draw_mode;


    /**
     *  VAO, VBOs and VBOs' length.
     */
    GLuint
        vertex_array_object,
        vertex_buffer,
        color_buffer,
        verts_length;


public:

    /**
     *  VertexColorBatch initialization.
     */
    VertexColorBatch ();


    /**
     *  Clean-up.
     */
    virtual ~VertexColorBatch ();


    /**
     *  ...
     */
    VertexColorBatch& prepare (
        GLenum,
        const std::vector<vec3> &,
        const std::vector<vec4> &
    );


    /**
     *  Draw VertexColorBatch contents.
     */
    virtual void draw () const;

};




/**
 *  Triangle batch.
 */
class TriangleBatch : public Batch {

    using vec2 = m3d::GVector2<GLfloat>;
    using vec3 = m3d::GVector3<GLfloat>;


protected:

    /**
     *  Number of allocated VBOs.
     */
    static const GLushort buff_amount { 4 };


    /**
     *  VAO, VBOs and VBO's lengths.
     */
    GLuint
        vertex_array_object,
        buffer[buff_amount],
        length[buff_amount];


public:

    /**
     *  TriangleBatch initialization.
     */
    TriangleBatch ();


    /**
     *  Clean-up.
     */
    virtual ~TriangleBatch ();


    /**
     *  ...
     */
    TriangleBatch& prepare (
        const std::vector<vec3> &,
        const std::vector<vec3> &,
        const std::vector<vec2> &,
        const std::vector<GLushort> &
    );


    /**
     *  Draw TriangleBatch contents.
     */
    virtual void draw () const;

};




} // namespace machina

#endif
