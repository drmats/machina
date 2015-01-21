/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __SHADER_HPP_
#define __SHADER_HPP_ 1

#include "sdl_opengl.hpp"
#include "m3d.hpp"
#include <stdexcept>
#include <string>
#include <tuple>
#include <initializer_list>

namespace machina {
    namespace shader {




/**
 *  Shader source helper macro.
 */
#define GLSL(version, src) "#version " #version "\n" #src




/**
 *  Generic attribute index names.
 */
enum attrib_index {
    vertex = 0,
    color = 1,
    normal = 2
};




/**
 *  Shader sources.
 */
extern const std::string vs_basic;
extern const std::string fs_basic_color_uniform;
extern const std::string vs_basic_attribute_color;
extern const std::string fs_basic_color_in;




/**
 *  ...
 */
class Shader {

    using vec4 = m3d::GVector4<GLfloat>;
    using mat4 = m3d::GMatrix4<GLfloat>;


private:

    /**
     *  OpenGL messages buffer.
     */
     static GLchar message_buffer[GL_INFO_LOG_LENGTH];


protected:

    // ...
    GLuint program = 0;


public:

    /**
     *  Initialization.
     */
    Shader (
        const std::string &, const std::string &,
        const std::initializer_list<std::tuple<attrib_index, std::string>>
    ) throw (std::runtime_error);


    /**
     *  Clean-up.
     */
    ~Shader ();


    /**
     *  ...
     */
    void use (const mat4 &) const;


    /**
     *  ...
     */
    void use_with_uniform_color (const mat4 &, const vec4 &) const;


    /**
     *  Compile shader from a given source.
     */
    GLuint load_shader (
        GLenum,
        const GLchar *
    ) throw (std::runtime_error);


    /**
     *  Get shader compilation log.
     */
    std::string get_shader_info_log (GLuint);


    /**
     *  Get program linking log.
     */
    std::string get_program_info_log (GLuint);

};




    } // namespace shader
} // namespace machina

#endif
