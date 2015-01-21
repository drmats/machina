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
const std::string vs_basic = GLSL(130,
    uniform mat4 mvp_matrix;
    in vec4 vertex_position;
    void main (void) {
        gl_Position = mvp_matrix * vertex_position;
    }
);

// basic fragment shader -- care about fragment color (from uniform)
const std::string fs_basic_color_uniform = GLSL(130,
    uniform vec4 uniform_color;
    out vec4 fragment_color;
    void main (void) {
        fragment_color = uniform_color;
    }
);

// basic vertex shader -- care about vertex position and color
const std::string vs_basic_attribute_color = GLSL(130,
    uniform mat4 mvp_matrix;
    in vec4 vertex_position;
    in vec4 vertex_color;
    smooth out vec4 vertex_fragment_color;
    void main (void) {
        vertex_fragment_color = vertex_color;
        gl_Position = mvp_matrix * vertex_position;
    }
);

// basic fragment shader -- care about fragment color (from vs)
const std::string fs_basic_color_in = GLSL(130,
    smooth in vec4 vertex_fragment_color;
    out vec4 fragment_color;
    void main (void) {
        fragment_color = vertex_fragment_color;
    }
);




/**
 *  OpenGL messages buffer.
 */
GLchar Shader::message_buffer[GL_INFO_LOG_LENGTH] = { 0 };




/**
 *  ...
 */
Shader::Shader (const std::string &vs, const std::string &fs) throw (std::runtime_error) {

    // temporary shader objects (load and compile from sources)
    GLuint
        vertex_shader = this->load_shader(GL_VERTEX_SHADER, (GLchar*)vs.data()),
        fragment_shader = this->load_shader(GL_FRAGMENT_SHADER, (GLchar*)fs.data());

    // status variable (for testing errors)
    GLint status;

    // attach shaders to gl program
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex_shader);
    glAttachShader(this->program, fragment_shader);

    // bind attributes to the gl program
    glBindAttribLocation(this->program, attrib_index::vertex, "vertex_position");
    glBindAttribLocation(this->program, attrib_index::color, "vertex_color");

    // link gl program
    glLinkProgram(this->program);

    // temporary shaders are no longer needed
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // check for link errors
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string error_message(this->get_program_info_log(this->program));
        glDeleteProgram(this->program);
        throw std::runtime_error(error_message);
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
void Shader::use (const mat4 &mvp) const {
    glUseProgram(this->program);
    glUniformMatrix4fv(
        glGetUniformLocation(this->program, "mvp_matrix"),
        1, GL_FALSE, *mvp
    );
}




/**
 *  ...
 */
void Shader::use_with_uniform_color (const mat4 &mvp, const vec4 &color) const {
    glUseProgram(this->program);
    glUniformMatrix4fv(
        glGetUniformLocation(this->program, "mvp_matrix"),
        1, GL_FALSE, *mvp
    );
    glUniform4fv(
        glGetUniformLocation(this->program, "uniform_color"),
        1, *color
    );
}




/**
 *  Compile shader from a given source.
 */
GLuint Shader::load_shader (
    GLenum shader_type,
    const GLchar *shader_src
) throw (std::runtime_error) {
    GLuint shader = glCreateShader(shader_type);
    GLint status;

    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string error_message(this->get_shader_info_log(shader));
        glDeleteShader(shader);
        throw std::runtime_error(error_message);
    }

    return shader;
}




/**
 *  Get shader compilation log message.
 */
std::string Shader::get_shader_info_log (GLuint shader) {
    glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, NULL, Shader::message_buffer);
    return "GLSL shader: " + std::string(Shader::message_buffer);
}




/**
 *  Get program linking log.
 */
std::string Shader::get_program_info_log (GLuint program) {
    glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, NULL, Shader::message_buffer);
    return "GLSL program: " + std::string(Shader::message_buffer);
}




    } // namespace shader
} // namespace machina

#endif
