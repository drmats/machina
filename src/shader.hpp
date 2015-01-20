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
 *  ...
 */
class Shader {

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
    Shader () throw (std::runtime_error);


    /**
     *  Clean-up.
     */
    ~Shader ();


    /**
     *  ...
     */
    void use (const mat4 &) const;


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
