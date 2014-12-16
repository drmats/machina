/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MACHINA_H_
#define __MACHINA_H_ 1

#include "sdl_opengl.h"
#include <iostream>
#include <cstdlib>
#include "main_loop.h"

namespace machina {




/**
 *  "Defines".
 */
const int VIDEO_DRIVER_NAME_BUFFER_SIZE = 64;
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


    // SDL library versions
    SDL_version
        compile_sdl_version,
        linked_sdl_version;


    // drawing surface
    SDL_Surface *screen;


    // current video driver
    char video_driver_name[VIDEO_DRIVER_NAME_BUFFER_SIZE];


    // viewport parameters
    struct view_t {
        int width = 960;
        int height = 540;
        int bpp = 32;
    } view;


    // event-processing / frame-rendering
    MainLoop main_loop;




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
     *  Setup OpenGL parameters.
     */
    void setup_opengl ();

};




} // namespace machina

#endif
