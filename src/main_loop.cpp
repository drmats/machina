/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_LOOP_CPP_
#define __MAIN_LOOP_CPP_ 1

#include "main_loop.h"
#include "machina.h"

namespace machina {




/**
 *  Initialize vital components.
 */
MainLoop::MainLoop (Machina *root):
    root{root}
{
    this->assign_default_handlers();
    this->setup_opengl();
}




/**
 *  Clean-up.
 */
MainLoop::~MainLoop () {}




/**
 *  Assign default handlers (mouse/keyboard).
 */
void MainLoop::assign_default_handlers () {
    // ...
}




/**
 *  Setup initial OpenGL parameters.
 */
void MainLoop::setup_opengl () {
    glClearColor(0.1f, 0.1f, 0.3f, 0.0f);
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    this->camera.projection.set_all([this] () {
        auto all = this->camera.projection.get_all();
        all[1] =
            static_cast< GLfloat >(this->root->viewport.width) /
            static_cast< GLfloat >(this->root->viewport.height);
        return all;
    }());
}




/**
 *  Event-processing.
 */
inline void MainLoop::process_events () {
    while (this->running  &&  SDL_PollEvent(&this->event)) {
        try {
            switch (this->event.type) {
                case SDL_MOUSEMOTION:
                    this->handle.mouse_motion(this->event);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    this->handle.mouse_buttons(this->event);
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    this->handle.keyboard(this->event);
                    break;

                case SDL_QUIT:
                    this->terminate();
                    break;
            }
        } catch (std::bad_function_call &e) {
            std::cout
                << "MainLoop::process_events: "
                << e.what()
                << std::endl;
        }
    }
}




/**
 *  Drawing.
 */
inline void MainLoop::draw () const {
    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );
    this->camera.establish_projection();
    this->camera.establish_modelview();
}




/**
 *  Main application loop.
 */
void MainLoop::run () {
    this->running = true;
    while (this->running) {
        this->process_events();
        this->draw();
        SDL_GL_SwapBuffers();
    }
}




/**
 *  Gracefully finish.
 */
void MainLoop::terminate () {
    this->running = false;
}




} // namespace machina

#endif
