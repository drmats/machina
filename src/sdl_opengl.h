/**
 *  machina
 *
 *  Copyright (c) 2014, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __SDL_OPENGL_H_
#define __SDL_OPENGL_H_ 1

#include "platform.h"




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




#endif