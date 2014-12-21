/**
 *  machina
 *
 *  Copyright (c) 2011, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __PLATFORM_H_
#define __PLATFORM_H_ 1




#if defined(linux) || defined(__linux) || defined(__linux__)
#undef __LINUX__
#define __LINUX__   1
#endif
#if defined(WIN32) || defined(_WIN32)
#undef __WIN32__
#define __WIN32__   1
#endif


#if defined(__WIN32__) && defined(_MSC_VER)
#pragma warning( disable : 4290 )
#endif




#endif
