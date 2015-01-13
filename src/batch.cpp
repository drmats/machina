/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __BATCH_CPP_
#define __BATCH_CPP_ 1

#include "batch.hpp"

namespace machina {




/**
 *  SimpleBatch initialization.
 */
SimpleBatch::SimpleBatch ():
    vertex_array(0),
    color_array(0)
    {}




/**
 *  Clean-up.
 */
SimpleBatch::~SimpleBatch () {
    if (this->vertex_array != 0) {
        glDeleteBuffers(1, &this->vertex_array);
    }
    if (this->color_array != 0) {
        glDeleteBuffers(1, &this->color_array);
    }
}




/**
 *  ...
 */
void SimpleBatch::prepare (
    GLenum draw_mode,
    GLuint num_verts,
    GLfloat *verts,
    GLfloat *colors
) {
    this->draw_mode = draw_mode;
    this->num_verts = num_verts;

    // vertex positions
    glGenBuffers(1, &this->vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_array);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(m3d::GVector3<GLfloat>) * num_verts,
        verts,
        GL_DYNAMIC_DRAW
    );

    // vertex colors
    glGenBuffers(1, &this->color_array);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_array);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(m3d::GVector4<GLfloat>) * num_verts,
        colors,
        GL_DYNAMIC_DRAW
    );
}




/**
 *  Draw SimpleBatch contents.
 */
void SimpleBatch::draw () const {
    // vertex positions buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // vertex colors buffer
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_array);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(this->draw_mode, 0, this->num_verts);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}




} // namespace machina

#endif
