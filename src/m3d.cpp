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

namespace m3d {




/**
 *  GArray destructor.
 */
template< typename T, std::size_t N >
GArray< T, N >::~GArray () {}




/**
 *  GArray deep comparision "==".
 */
template< typename T, std::size_t N >
bool operator== (const GArray < T, N > &l, const GArray< T, N > &r) {
    for (std::size_t i = 0;  i < N;  i++) {
        if (l[i] != r[i]) { return false; }
    }
    return true;
}




/**
 *  GArray deep comparision "!=".
 */
template< typename T, std::size_t N >
bool operator!= (const GArray < T, N > &l, const GArray< T, N > &r) {
    return !(l == r);
}




/**
 *  GArray to string serialization.
 */
template< typename T, std::size_t N >
std::ostream& operator<< (std::ostream &os, const GArray< T, N > &gv) {
    os << "[";
    for (std::size_t i = 0;  i < N;  i++) {
        os << gv[i];
        if (i < N-1) { os << ", "; }
    }
    os << "]";
    return os;
}




} // namespace m3d

#endif
