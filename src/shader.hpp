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

namespace machina {
    namespace shader {




/**
 *  ...
 */
class Shader {

protected:

    // ...
    GLuint program = 0;


public:

    /**
     *  Initialization.
     */
    Shader ();


    /**
     *  Clean-up.
     */
    ~Shader ();


    /**
     *  ...
     */
    void use (const m3d::GMatrix4<GLfloat> &mvp, const GLfloat *color) const;


    /**
     *  Compile shader from a given source.
     */
    GLuint load_shader (
        GLenum shader_type,
        const GLchar *shader_src
    ) throw (std::runtime_error);

};




    } // namespace shader
} // namespace machina

#endif
