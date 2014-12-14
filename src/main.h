/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_H_
#define __MAIN_H_ 1

#include "m3d.h"
#include "camera.h"

#include <iostream>
#include <cstdlib>

#include "sdl_opengl.h"

using namespace std;
using namespace m3d;




#define DRIVER_NAME_BUFFER_SIZE 64
#define PROGRAM_NAME "machina"




/**
 *  Main program class.
 */
class Main {

private:

    // command-line parameters
    int argc;
    char **argv;


    // SDL library versions
    SDL_version
        compileSDLversion,
        linkedSDLversion;


    // drawing surface
    SDL_Surface *screen;


    // event storage
    SDL_Event event;


    // current video driver
    char videoDriverName[DRIVER_NAME_BUFFER_SIZE];


    // viewport parameters
    struct view_t {
        int width = 960;
        int height = 540;
        int bpp = 32;
    } view;


    // main camera
    Camera< GLfloat > camera;


    // main loop sustaining variable
    bool mainLoopRunning = true;




public:

    /**
     *  Initialize vital application components.
     */
    Main (int argc, char *argv[]);


    /**
     *  Clean-up.
     */
    ~Main ();


    /**
     *  Obtain SDL versions, initialize SDL and get video driver name.
     */
    void initializeSDL ();


    /**
     *  Initialize viewport/drawing surface.
     */
    void initializeSurface ();


    /**
     *  Event-processing.
     */
    inline void processEvents ();


    /**
     *  Setup OpenGL parameters.
     */
    void setupOpenGL ();


    /**
     *  Drawing.
     */
    inline void draw ();


    /**
     *  Main application loop.
     */
    void mainLoop ();

};

#endif
