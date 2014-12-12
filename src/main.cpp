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
    #define DRIVER_NAME_BUFFER_SIZE 64
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
    Main (int argc, char *argv[]):
        argc{argc}, argv{argv} {

        cout << "Hi!" << endl;

        this->initializeSDL();
        this->initializeSurface();
        this->setupOpenGL();
        this->mainLoop();
    }




    /**
     *  Clean-up.
     */
    ~Main () {
        cout << "Bye!" << endl;
    }




    /**
     *  Obtain SDL versions, initialize SDL and get video driver name.
     */
    void initializeSDL () {
        SDL_VERSION(&this->compileSDLversion);
        cout
            << "Compile-time SDL version: "
            << static_cast<int>(this->compileSDLversion.major) << "."
            << static_cast<int>(this->compileSDLversion.minor) << "."
            << static_cast<int>(this->compileSDLversion.patch)
            << endl;

        this->linkedSDLversion = *SDL_Linked_Version();
        cout
            << "Runtime SDL version: "
            << static_cast<int>(this->linkedSDLversion.major) << "."
            << static_cast<int>(this->linkedSDLversion.minor) << "."
            << static_cast<int>(this->linkedSDLversion.patch)
            << endl;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            cerr
                << "Unable to initialize SDL: "
                << SDL_GetError()
                << endl;
            exit(EXIT_FAILURE);
        }

        atexit(SDL_Quit);

        if (
            SDL_VideoDriverName(
                this->videoDriverName,
                DRIVER_NAME_BUFFER_SIZE
            ) != NULL
        ) {
            cout
                << "Video driver: "
                << this->videoDriverName
                << endl;
        } else {
            cerr
                << "Failed to obtain the video driver name."
                << endl;
        }
    }




    /**
     *  Initialize viewport/drawing surface.
     */
    void initializeSurface () {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        this->screen = SDL_SetVideoMode(
            this->view.width, this->view.height, this->view.bpp,
            SDL_OPENGL
            | SDL_GL_DOUBLEBUFFER
            | SDL_HWPALETTE
            | SDL_HWSURFACE
            | SDL_HWACCEL
        );

        if (screen != NULL) {
            cout
                << "Initialized surface: "
                << this->screen->w << "x" << this->screen->h
                << "@" << static_cast<int>(this->screen->format->BitsPerPixel)
                << endl;
        } else {
            cerr
                << "Unable to initialize surface: "
                << SDL_GetError()
                << endl;
            exit(EXIT_FAILURE);
        }

        glViewport(0, 0, this->screen->w, this->screen->h);
        SDL_WM_SetCaption("machina", NULL);
    }




    /**
     *  Event-processing.
     */
    inline void processEvents () {
        while (this->mainLoopRunning  &&  SDL_PollEvent(&this->event)) {
            switch (this->event.type) {
                case SDL_QUIT:
                    this->mainLoopRunning = false;
                    break;
            }
        }
    }




    /**
     *  Setup OpenGL parameters.
     */
    void setupOpenGL () {
        glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
        glPolygonMode(GL_FRONT, GL_FILL);
        glShadeModel(GL_SMOOTH);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
    }




    /**
     *  Drawing.
     */
    inline void draw () {
        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );
    }




    /**
     *  Main application loop.
     */
    void mainLoop () {
        while (this->mainLoopRunning) {
            this->processEvents();
            this->draw();
            SDL_GL_SwapBuffers();
        }
    }

};




/**
 *  Entry-point.
 */
int main (int argc, char *argv[]) {
    Main *m = new Main(argc, argv);
    delete m;
    exit(EXIT_SUCCESS);
}
