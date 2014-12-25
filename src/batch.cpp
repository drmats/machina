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
    vertex_array(0) {

}




/**
 *  Clean-up.
 */
SimpleBatch::~SimpleBatch () {
    if (this->vertex_array != 0) {
        glDeleteBuffers(1, &this->vertex_array);
    }
}




/**
 *  ...
 */
void SimpleBatch::prepare (
    GLenum draw_mode,
    GLuint num_verts,
    GLfloat *verts
) {
    this->draw_mode = draw_mode;
    this->num_verts = num_verts;

    glGenBuffers(1, &this->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_array);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(m3d::GVector3<GLfloat>) * num_verts,
        verts,
        GL_DYNAMIC_DRAW
    );
}




/**
 *  Draw SimpleBatch contents.
 */
void SimpleBatch::draw () const {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(this->draw_mode, 0, this->num_verts);

    glDisableVertexAttribArray(0);
}




} // namespace machina

#endif
