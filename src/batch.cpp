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
 *  VertexBatch initialization.
 */
VertexBatch::VertexBatch ():
    vertex_array_object{0},
    vertex_buffer{0},
    color_buffer{0}
    {}




/**
 *  Clean-up.
 */
VertexBatch::~VertexBatch () {
    if (this->color_buffer != 0) {
        glDeleteBuffers(1, &this->color_buffer);
    }
    if (this->vertex_buffer != 0) {
        glDeleteBuffers(1, &this->vertex_buffer);
    }
    if (this->vertex_array_object != 0) {
        glDeleteVertexArrays(1, &this->vertex_array_object);
    }
}




/**
 *  ...
 */
VertexBatch& VertexBatch::prepare (
    GLenum draw_mode,
    const std::vector<vec3> &verts,
    const std::vector<vec4> &colors
) {
    this->draw_mode = draw_mode;
    this->num_verts = verts.size();

    // VAO
    glGenVertexArrays(1, &this->vertex_array_object);
    glBindVertexArray(this->vertex_array_object);

    // vertex positions
    glGenBuffers(1, &this->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        verts.size() * sizeof(vec3),
        verts.data(),
        GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // vertex colors
    glGenBuffers(1, &this->color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        colors.size() * sizeof(vec4),
        colors.data(),
        GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    return *this;
}




/**
 *  Draw VertexBatch contents.
 */
void VertexBatch::draw () const {
    glBindVertexArray(this->vertex_array_object);
    glDrawArrays(this->draw_mode, 0, this->num_verts);
}




} // namespace machina

#endif
