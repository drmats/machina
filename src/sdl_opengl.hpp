/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __SDL_OPENGL_HPP_
#define __SDL_OPENGL_HPP_ 1

#include "platform.hpp"




#ifdef __LINUX__
#include <SDL2/SDL.h>
#include <GL/glew.h>
#endif
#ifdef __WIN32__
#include <tchar.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#endif




#endif
