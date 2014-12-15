/**
 *  machina
 *
 *  Copyright (c) 2011, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __M3D_CPP_
#define __M3D_CPP_ 1

#include "m3d.h"
#include "sdl_opengl.h"

namespace m3d {




/**
 *  GArray destructor.
 */
template < typename T, std::size_t N >
GArray< T, N >::~GArray () {}




/**
 *  Instantiation of GArray destructor for allowed types.
 */
template GArray< GLfloat, 3 >::~GArray();
template GArray< GLdouble, 3 >::~GArray();
template GArray< GLfloat, 4 >::~GArray();
template GArray< GLdouble, 4 >::~GArray();
template GArray< GLfloat, 4*4 >::~GArray();
template GArray< GLdouble, 4*4 >::~GArray();




/**
 *  GArray deep comparision "==".
 */
template < typename T, std::size_t N >
bool operator== (const GArray < T, N > &l, const GArray< T, N > &r) {
    for (std::size_t i = 0;  i < N;  i++) {
        if (l[i] != r[i]) { return false; }
    }
    return true;
}




/**
 *  Instantiation of GArray deep comparision "==" for allowed types.
 */
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




/**
 *  GArray deep comparision "!=".
 */
template < typename T, std::size_t N >
bool operator!= (const GArray < T, N > &l, const GArray< T, N > &r) {
    return !(l == r);
}




/**
 *  Instantiation of GArray deep comparision "!=" for allowed types.
 */
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




/**
 *  GArray to string serialization.
 */
template < typename T, std::size_t N >
std::ostream& operator<< (std::ostream &os, const GArray< T, N > &gv) {
    os << "[";
    for (std::size_t i = 0;  i < N;  i++) {
        os << gv[i];
        if (i < N-1) { os << ", "; }
    }
    os << "]";
    return os;
}




/**
 *  Instantiation of GArray to string serialization for allowed types.
 */
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

#endif
