/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __SHADER_CPP_
#define __SHADER_CPP_ 1

#include "shader.hpp"

namespace machina {
    namespace shader {




// basic vertex shader -- care about vertex position
static const GLchar *vs_basic =
    "uniform mat4 mvp_matrix;\n"
    "attribute vec4 vertex_position;\n"
    "void main (void) {\n"
        "gl_Position = mvp_matrix * vertex_position;\n"
    "}\n";


// basic fragment shader -- care about fragment color
static const GLchar *fs_basic =
    "uniform vec4 color;\n"
    "void main (void) {\n"
        "gl_FragColor = color;\n"
    "}\n";




/**
 *  ...
 */
Shader::Shader () {

    // temporary shader objects
    GLuint
        vertex_shader = glCreateShader(GL_VERTEX_SHADER),
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // status variable (for testing errors)
    GLint status;

    // load shader sources
    this->load_shader_src(vs_basic, vertex_shader);
    this->load_shader_src(fs_basic, fragment_shader);

    // compile them
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    // check for compilation errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    // attach shaders to gl program
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex_shader);
    glAttachShader(this->program, fragment_shader);

    // bind attributes to the gl program
    glBindAttribLocation(this->program, 0, "vertex_position");

    // link gl program
    glLinkProgram(this->program);

    // temporary shaders are no longer needed
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // check for link errors
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteProgram(this->program);
    }
}




/**
 *  ...
 */
Shader::~Shader () {
    if (this->program != 0) {
        glDeleteProgram(this->program);
        this->program = 0;
    }
}




/**
 *  ...
 */
void Shader::use (const m3d::GMatrix4x4<GLfloat> &mvp, const GLfloat *color) const {
    glUseProgram(this->program);
    glUniformMatrix4fv(
        glGetUniformLocation(this->program, "mvp_matrix"),
        1, GL_FALSE, *mvp
    );
    glUniform4fv(
        glGetUniformLocation(this->program, "color"),
        1, color
    );
}




/**
 *  ...
 */
void Shader::load_shader_src (const GLchar *shader_src, GLuint shader) {
    const GLchar *shaders_source_table[] = { shader_src };
    glShaderSource(shader, 1, shaders_source_table, NULL);
}




    } // namespace shader
} // namespace machina

#endif
