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
#include <functional>

namespace machina {




/**
 *  Shader loader.
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

    /**
     *  Shader program handle.
     */
    GLuint program { 0 };


public:

    /**
     *  Generic attribute index names.
     */
    enum attrib_index : GLushort {
        vertex = 0,
        normal = 1,
        uv = 2,
        color = 3
    };


    /**
     *  Some shader sources.
     */
    static const std::string vs_basic;
    static const std::string fs_basic_color_uniform;
    static const std::string vs_basic_attribute_color;
    static const std::string fs_basic_color_in;


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
    Shader (
        const std::string &, const std::string &,
        const std::initializer_list<
            std::tuple<std::string, Shader::attrib_index>
        >
    ) throw (std::runtime_error);


    /**
     *  Clean-up.
     */
    ~Shader ();


    /**
     *  Assign uniforms and use shader.
     *  Example invocation:
     *      some_shader.use({
     *          std::make_tuple("mvp_matrix", [&] (GLuint location) {
     *              glUniformMatrix4fv(location, 1, GL_FALSE, *mvp_matrix);
     *          })
     *      });
     */
    const Shader& use (
        std::initializer_list<
            std::tuple<std::string, std::function<void (GLint)>>
        >
    ) const;


    /**
     *  Compile shader from a given source.
     */
    GLuint load_shader (
        GLenum,
        const std::string &
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




} // namespace machina

#endif
