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
#include <functional>
#include <vector>
#include <memory>
#include <chrono>
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

    using vec2 = m3d::GVector2<GLfloat>;
    using vec3 = m3d::GVector3<GLfloat>;
    using vec4 = m3d::GVector4<GLfloat>;
    using mat4 = m3d::GMatrix4<GLfloat>;


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
        void look_around_target_camera (MainLoop *, const SDL_Event &, const GLfloat);
        void look_around_camera (MainLoop *, const SDL_Event &);
        void move_around_camera (MainLoop *, const SDL_Event &);
        void mouse_wheel (MainLoop *, const SDL_Event &);
        void mouse_buttons (MainLoop *, const SDL_Event &);
        void keyboard (MainLoop *, const SDL_Event &);
        void window (MainLoop *, const SDL_Event &);
    } default_handler;


    // necessary time variables
    std::chrono::steady_clock::time_point time_mark;
    GLulong
        elapsed_time,
        total_time;


    // main camera
    Camera<GLfloat> camera;


    // simple "scene"
    std::vector<std::shared_ptr<Batch>> scene;


    // shaders
    Shader
        vertex_color_attrib_shader,
        all_attrib_shader;


    // loop sustaining variable
    bool running { false };




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
