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




/**
 *  Shader source helper macro.
 */
#define GLSL(version, src) "#version " #version "\n" #src




/**
 *  OpenGL messages buffer.
 */
GLchar Shader::message_buffer[GL_INFO_LOG_LENGTH] { 0 };




/**
 *  Some shader sources.
 */
// basic vertex shader -- care about vertex position
const std::string Shader::vs_basic = GLSL(130,
    uniform mat4 mvp_matrix;
    in vec3 vertex_position;

    void main (void) {
        gl_Position = mvp_matrix * vec4(vertex_position, 1);
    }
);


// basic fragment shader -- care about fragment color (from uniform)
const std::string Shader::fs_basic_color_uniform = GLSL(130,
    uniform vec4 uniform_color;
    out vec4 fragment_color;

    void main (void) {
        fragment_color = uniform_color;
    }
);


// basic vertex shader -- care about vertex position and color
const std::string Shader::vs_basic_attribute_color = GLSL(130,
    uniform mat4 mvp_matrix;
    in vec3 vertex_position;
    in vec4 vertex_color;
    smooth out vec4 vertex_fragment_color;

    void main (void) {
        vertex_fragment_color = vertex_color;
        gl_Position = mvp_matrix * vec4(vertex_position, 1);
    }
);


// basic fragment shader -- care about fragment color (from vs)
const std::string Shader::fs_basic_color_in = GLSL(130,
    smooth in vec4 vertex_fragment_color;
    out vec4 fragment_color;

    void main (void) {
        fragment_color = vertex_fragment_color;
    }
);


// all attrib vertex shader
const std::string Shader::vs_all_attrib = GLSL(130,
    precision highp float;
    in vec3 vert_position;
    in vec3 vert_normal;
    in vec2 vert_uv;
    uniform mat4 mv_matrix;
    uniform mat4 p_matrix;
    smooth out vec3 frag_normal;
    smooth out vec3 frag_position;
    smooth out vec3 frag_mv_position;

    void main (void) {
        vec4 mv_position = mv_matrix * vec4(vert_position, 1);
        mat3 normal_matrix;

        normal_matrix[0] = mv_matrix[0].xyz;
        normal_matrix[1] = mv_matrix[1].xyz;
        normal_matrix[2] = mv_matrix[2].xyz;

        frag_position = vert_position;
        frag_mv_position = mv_position.xyz;
        frag_normal = normalize(normal_matrix * vert_normal);

        gl_Position = p_matrix * mv_position;
    }
);


// all attrib fragment shader
const std::string Shader::fs_all_attrib = GLSL(130,
    precision highp float;
    smooth in vec3 frag_position;
    smooth in vec3 frag_mv_position;
    smooth in vec3 frag_normal;

    void main (void) {
        gl_FragColor = vec4(frag_normal, 1);
    }
);




/**
 *  Initialization with attribute binding.
 *  Example invocation:
 *      Shader some_shader {
 *          shader::vs_basic,
 *          shader::fs_basic, {
 *              std::make_tuple(
 *                  "vertex_position", Shader::attrib_index::vertex
 *              )
 *      } };
 */
Shader::Shader (
    const std::string &vs, const std::string &fs,
    const std::initializer_list<
        std::tuple<std::string, Shader::attrib_index>
    > binding
) throw (std::runtime_error) {
    // temporary shader objects (load and compile from sources)
    GLuint
        vertex_shader { this->load_shader(GL_VERTEX_SHADER, vs) },
        fragment_shader { this->load_shader(GL_FRAGMENT_SHADER, fs) };

    // status variable (for testing errors)
    GLint status;

    // attach shaders to gl program
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex_shader);
    glAttachShader(this->program, fragment_shader);

    // bind attributes to the gl program
    for (auto it = binding.begin();  it != binding.end();  it++) {
        glBindAttribLocation(
            this->program,
            std::get<1>(*it),
            std::get<0>(*it).data()
        );
    }

    // link gl program
    glLinkProgram(this->program);

    // temporary shaders are no longer needed
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // check for link errors
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string error_message {
            this->get_program_info_log(this->program)
        };
        glDeleteProgram(this->program);
        throw std::runtime_error(error_message);
    }
}




/**
 *  Clean-up.
 */
Shader::~Shader () {
    if (this->program != 0) {
        glDeleteProgram(this->program);
        this->program = 0;
    }
}




/**
 *  Assign uniforms and use shader.
 *  Example invocation:
 *      some_shader.use({
 *          std::make_tuple("mvp_matrix", [&] (GLuint location) {
 *              glUniformMatrix4fv(location, 1, GL_FALSE, *mvp_matrix);
 *          })
 *      });
 */
const Shader& Shader::use (
    std::initializer_list<
        std::tuple<std::string, std::function<void (GLint)>>
    > uniforms
) const {
    glUseProgram(this->program);

    for (auto it = uniforms.begin();  it != uniforms.end();  it++) {
        std::get<1>(*it)(
            glGetUniformLocation(
                this->program, (GLchar*)std::get<0>(*it).data()
            )
        );
    }

    return *this;
}




/**
 *  Compile shader from a given source.
 */
GLuint Shader::load_shader (
    GLenum shader_type,
    const std::string &shader_src
) throw (std::runtime_error) {
    GLuint shader { glCreateShader(shader_type) };
    const GLchar *src { (GLchar*)shader_src.data() };
    GLint status;

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string error_message {
            this->get_shader_info_log(shader)
        };
        glDeleteShader(shader);
        throw std::runtime_error(error_message);
    }

    return shader;
}




/**
 *  Get shader compilation log message.
 */
std::string Shader::get_shader_info_log (GLuint shader) {
    glGetShaderInfoLog(
        shader, GL_INFO_LOG_LENGTH, NULL, Shader::message_buffer
    );
    return "GLSL shader: " + std::string(Shader::message_buffer);
}




/**
 *  Get program linking log.
 */
std::string Shader::get_program_info_log (GLuint program) {
    glGetProgramInfoLog(
        program, GL_INFO_LOG_LENGTH, NULL, Shader::message_buffer
    );
    return "GLSL program: " + std::string(Shader::message_buffer);
}




} // namespace machina

#endif
