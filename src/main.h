/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#include "m3d.h"

#include <iostream>
#include <cstdlib>

#ifdef __LINUX__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif
#ifdef __WIN32__
#include <tchar.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_opengl.h>
#endif

using namespace std;
using namespace m3d;




#define DRIVER_NAME_BUFFER_SIZE 64




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
