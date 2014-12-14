/**
 *  machina
 *
 *  Copyright (c) 2011, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#include "m3d.cpp"

#ifdef __LINUX__
#include <SDL/SDL_opengl.h>
#endif
#ifdef __WIN32__
#include <SDL_opengl.h>
#endif

namespace m3d {




template bool operator== < GLfloat, 3 > (
    const GArray < GLfloat, 3 > &l, const GArray< GLfloat, 3 > &r
);
template bool operator== < GLdouble, 3 > (
    const GArray < GLdouble, 3 > &l, const GArray< GLdouble, 3 > &r
);
template bool operator== < GLfloat, 4 > (
    const GArray < GLfloat, 4 > &l, const GArray< GLfloat, 4 > &r
);
template bool operator== < GLdouble, 4 > (
    const GArray < GLdouble, 4 > &l, const GArray< GLdouble, 4 > &r
);
template bool operator== < GLfloat, 4*4 > (
    const GArray < GLfloat, 4*4 > &l, const GArray< GLfloat, 4*4 > &r
);
template bool operator== < GLdouble, 4*4 > (
    const GArray < GLdouble, 4*4 > &l, const GArray< GLdouble, 4*4 > &r
);




template bool operator!= < GLfloat, 3 > (
    const GArray < GLfloat, 3 > &l, const GArray< GLfloat, 3 > &r
);
template bool operator!= < GLdouble, 3 > (
    const GArray < GLdouble, 3 > &l, const GArray< GLdouble, 3 > &r
);
template bool operator!= < GLfloat, 4 > (
    const GArray < GLfloat, 4 > &l, const GArray< GLfloat, 4 > &r
);
template bool operator!= < GLdouble, 4 > (
    const GArray < GLdouble, 4 > &l, const GArray< GLdouble, 4 > &r
);
template bool operator!= < GLfloat, 4*4 > (
    const GArray < GLfloat, 4*4 > &l, const GArray< GLfloat, 4*4 > &r
);
template bool operator!= < GLdouble, 4*4 > (
    const GArray < GLdouble, 4*4 > &l, const GArray< GLdouble, 4*4 > &r
);




template std::ostream& operator<< < GLfloat, 3 > (
    std::ostream &os, const GArray< GLfloat, 3 > &gv
);
template std::ostream& operator<< < GLdouble, 3 > (
    std::ostream &os, const GArray< GLdouble, 3 > &gv
);
template std::ostream& operator<< < GLfloat, 4 > (
    std::ostream &os, const GArray< GLfloat, 4 > &gv
);
template std::ostream& operator<< < GLdouble, 4 > (
    std::ostream &os, const GArray< GLdouble, 4 > &gv
);
template std::ostream& operator<< < GLfloat, 4*4 > (
    std::ostream &os, const GArray< GLfloat, 4*4 > &gv
);
template std::ostream& operator<< < GLdouble, 4*4 > (
    std::ostream &os, const GArray< GLdouble, 4*4 > &gv
);




} // namespace m3d
