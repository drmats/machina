/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MACHINA_HPP_
#define __MACHINA_HPP_ 1

#include "sdl_opengl.hpp"
#include <iostream>
#include <cstdlib>
#include <memory>
#include "main_loop.hpp"

namespace machina {




/**
 *  "Defines".
 */
const std::string PROGRAM_NAME = "machina";




/**
 *  Main program class.
 */
class Machina {

protected:

    // command-line parameters
    struct command_line_params_t {
        command_line_params_t (
            int const& argc,
            char** const& argv
        ):
            argc{argc}, argv{argv}
            {}
        int const& argc;
        char** const& argv;
    } command_line_params;


    // ...
    static const int init_delay = 150;


    // SDL library versions
    SDL_version
        compile_sdl_version,
        linked_sdl_version;


    // current video driver
    const char *video_driver_name;


    // drawing surface
    SDL_Window *main_window;
    SDL_GLContext gl_context;


    // OpenGL version
    GLint
        opengl_major_version,
        opengl_minor_version;
    const char *opengl_version_string;


    // viewport parameters
    struct {
        int width = 960;
        int height = 540;
    } viewport;


    // event-processing / frame-rendering
    std::shared_ptr<MainLoop> main_loop;
    friend class MainLoop;




public:

    /**
     *  Initialize vital application components.
     */
    Machina (int argc, char **argv);


    /**
     *  Clean-up.
     */
    ~Machina ();


    /**
     *  Obtain SDL versions, initialize SDL and get video driver name.
     */
    void initialize_sdl ();


    /**
     *  Initialize viewport/drawing surface.
     */
    void initialize_surface ();


    /**
     *  Run the app.
     */
    void run ();

};




} // namespace machina

#endif
