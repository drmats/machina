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
#include "shader.hpp"

namespace machina {




/**
 *  VertexColorBatch initialization.
 */
VertexColorBatch::VertexColorBatch ():
    vertex_array_object{0},
    vertex_buffer{0},
    color_buffer{0},
    verts_length{0}
    {}




/**
 *  Clean-up.
 */
VertexColorBatch::~VertexColorBatch () {
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
VertexColorBatch& VertexColorBatch::prepare (
    GLenum draw_mode,
    const std::vector<vec3> &verts,
    const std::vector<vec4> &colors
) {
    this->draw_mode = draw_mode;
    this->verts_length = verts.size();

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
    glEnableVertexAttribArray(Shader::attrib_index::vertex);
    glVertexAttribPointer(
        Shader::attrib_index::vertex,
        3, GL_FLOAT, GL_FALSE, 0, 0
    );

    // vertex colors
    glGenBuffers(1, &this->color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        colors.size() * sizeof(vec4),
        colors.data(),
        GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(Shader::attrib_index::color);
    glVertexAttribPointer(
        Shader::attrib_index::color,
        4, GL_FLOAT, GL_FALSE, 0, 0
    );

    glBindVertexArray(0);

    return *this;
}




/**
 *  Draw VertexColorBatch contents.
 */
void VertexColorBatch::draw () const {
    glBindVertexArray(this->vertex_array_object);
    glDrawArrays(this->draw_mode, 0, this->verts_length);
}




/**
 *  TriangleBatch initialization.
 */
TriangleBatch::TriangleBatch ():
    vertex_array_object{0},
    buffer{0}, length{0}
    {}




/**
 *  Clean-up.
 */
TriangleBatch::~TriangleBatch () {
    glDeleteBuffers(this->buff_amount, this->buffer);
    if (this->vertex_array_object != 0) {
        glDeleteVertexArrays(1, &this->vertex_array_object);
    }
}




/**
 *  ...
 */
TriangleBatch& TriangleBatch::prepare (
    const std::vector<vec3> &verts,
    const std::vector<vec3i> &faces
) {

    this->length[Batch::buf_index::verts] = verts.size();
    this->length[Batch::buf_index::faces] = faces.size();

    // VAO -- generate and bind
    glGenVertexArrays(1, &this->vertex_array_object);
    glBindVertexArray(this->vertex_array_object);

    // generate VBOs
    glGenBuffers(this->buff_amount, this->buffer);

    // vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer[Batch::buf_index::verts]);
    glBufferData(
        GL_ARRAY_BUFFER,
        this->length[Batch::buf_index::verts] * sizeof(vec3),
        verts.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(Shader::attrib_index::vertex);
    glVertexAttribPointer(
        Shader::attrib_index::vertex,
        3, GL_FLOAT, GL_FALSE, 0, 0
    );

    // faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer[Batch::buf_index::faces]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        this->length[Batch::buf_index::faces] * sizeof(vec3i),
        faces.data(),
        GL_STATIC_DRAW
    );

    // VAO -- unbind
    glBindVertexArray(0);

    return *this;
}




/**
 *  Draw TriangleBatch contents.
 */
void TriangleBatch::draw () const {
    glBindVertexArray(this->vertex_array_object);
    glDrawElements(
        GL_TRIANGLES,
        this->length[Batch::buf_index::faces] * 3,
        GL_UNSIGNED_SHORT,
        0
    );
}




} // namespace machina

#endif
