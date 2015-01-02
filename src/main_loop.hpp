/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __MAIN_LOOP_HPP_
#define __MAIN_LOOP_HPP_ 1

#include "sdl_opengl.hpp"
#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include "camera.hpp"
#include "batch.hpp"
#include "shader.hpp"

namespace machina {




/**
 *  Forward declaration for "root" pointer.
 */
class Machina;




/**
 *  Work-horse.
 */
class MainLoop {

private:

    // program root
    Machina *root;


    // event storage
    SDL_Event event;


    // event handles
    struct {
        std::function<void (const SDL_Event &)> mouse_motion =
            [] (const SDL_Event &) -> void { return; };
        std::function<void (const SDL_Event &)> mouse_wheel =
            [] (const SDL_Event &) -> void { return; };
        std::function<void (const SDL_Event &)> mouse_buttons =
            [] (const SDL_Event &) -> void { return; };
        std::function<void (const SDL_Event &)> keyboard =
            [] (const SDL_Event &) -> void { return; };
        std::function<void (const SDL_Event &)> window =
            [] (const SDL_Event &) -> void { return; };
    } handle;


    // default event handlers
    struct default_handler_t {
        void empty_mouse_motion (MainLoop *, const SDL_Event &);
        void look_around_camera (MainLoop *, const SDL_Event &, const GLfloat);
        void move_around_camera (MainLoop *, const SDL_Event &);
        void mouse_wheel (MainLoop *, const SDL_Event &);
        void mouse_buttons (MainLoop *, const SDL_Event &);
        void keyboard (MainLoop *, const SDL_Event &);
        void window (MainLoop *, const SDL_Event &);
    } default_handler;


    // main camera
    Camera<GLfloat> camera;


    // batch
    SimpleBatch model;


    // shader
    shader::Shader shader;


    // loop sustaining variable
    bool running = false;




public:

    /**
     *  Initialize vital components.
     */
    MainLoop (Machina *);


    /**
     *  Assign default handlers (mouse/keyboard).
     */
    void assign_default_handlers ();


    /**
     *  Adjust camera aspect ratio.
     */
    inline void adjust_camera_aspect ();


    /**
     *  Setup initial OpenGL parameters.
     */
    void setup_opengl ();


    /**
     *  Event-processing.
     */
    inline void process_events ();


    /**
     *  Drawing.
     */
    inline void draw () const;


    /**
     *  Main application loop.
     */
    void run ();


    /**
     *  Gracefully finish.
     */
    void terminate ();

};




} // namespace machina

#endif
