/**
 *  machina
 *
 *  Copyright (c) 2011, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __M3D_H_
#define __M3D_H_ 1

#include "platform.h"
#include "messages.h"
#include <cstring>
#include <stdexcept>
#include <ostream>
#include <cmath>

namespace m3d {




/**
 *  Useful constants definition.
 */
#define m3d_pi       3.14159265358979323846264338327950
#define m3d_twopi    6.28318530717958647692528676655900
#define m3d_pi2      1.57079632679489661923132169163980
#define m3d_pi3      1.04719755119659774615421446109320
#define m3d_pi4      0.78539816339744830961566084581988
#define m3d_pi6      0.52359877559829887307710723054658
#define m3d_pi180    0.01745329251994329576923690768488
#define m3d_invpi180 57.2957795130823208767981548141050

#define m3d_epsilon  1e-6




/**
 *  Equality test with allowed error.
 */
template < typename T >
inline bool close_to (T a, T b) {
    return fabs(a - b) < static_cast<T>(m3d_epsilon);
}




/**
 *  Square root.
 */
template < typename T >
inline T sqr (T x) {
    return x * x;
}




/**
 *  Angle conversions.
 */
template < typename T >
inline T deg_to_rad (T x) {
    return x * static_cast<T>(m3d_pi180);
}

template < typename T >
inline T rad_to_deg (T x) {
    return x * static_cast<T>(m3d_invpi180);
}




/**
 *  Linear interpolation.
 */
template < typename T >
inline T linear_interpolation (T x1, T x2, T x3, T y1, T y3) {
    return (((x2 - x1) * (y3 - y1)) / (x3 - x1)) + y1;
}




/**
 *  Forward declarations for GArray friend operators.
 */
template < typename T, std::size_t N >
class GArray;

template < typename T, std::size_t N >
bool operator== (const GArray < T, N > &, const GArray< T, N > &);

template < typename T, std::size_t N >
bool operator!= (const GArray < T, N > &, const GArray< T, N > &);

template < typename T, std::size_t N >
std::ostream& operator<< (std::ostream &, const GArray< T, N > &);




/**
 *  Base abstract class for Points, Vectors and Matrices.
 */
template < typename T, std::size_t N >
class GArray {

protected:

    T data[N];


public:

    virtual ~GArray () = 0;


    /**
     *  Array indexers.
     */
    T& operator[] (std::size_t i) throw (std::out_of_range) {
        if (i >= N) {
            throw std::out_of_range(message::out_of_range);
        }
        return data[i];
    }

    const T& operator[] (std::size_t i) const throw (std::out_of_range) {
        if (i >= N) {
            throw std::out_of_range(message::out_of_range);
        }
        return data[i];
    }


    /**
     *  Return pointer of basic type to array.
     */
    const T* operator* () const { return data; }


    /**
     *  Return size of array.
     */
    std::size_t size () const { return N; }


    /**
     *  Reset current array (fill data with zeros).
     */
    GArray< T, N >& reset () {
        memset(data, 0, N*sizeof(T));
        return *this;
    }


    /**
     *  Replace current array.
     */
    GArray< T, N >& assign (const T d[]) {
        memcpy(data, d, N*sizeof(T));
        return *this;
    }

    GArray< T, N >& assign (const GArray< T, N > &a) {
        memcpy(data, *a, N*sizeof(T));
        return *this;
    }


    /**
     *  Add to current array.
     */
    GArray< T, N >& add (const GArray< T, N > &a) {
        for (std::size_t i = 0;  i < N;  i++) {
            data[i] += a[i];
        }
        return *this;
    }


    /**
     *  Subtract from current array.
     */
    GArray< T, N >& sub (const GArray< T, N > &a) {
        for (std::size_t i = 0;  i < N;  i++) {
            data[i] -= a[i];
        }
        return *this;
    }


    /**
     *  Scale current array (multiply each component by a given value).
     */
    GArray< T, N >& scale (T s) {
        for (std::size_t i = 0;  i < N;  i++) {
            data[i] *= s;
        }
        return *this;
    }


    /**
     *  GArray deep comparision.
     */
    friend bool operator== <> (const GArray < T, N > &, const GArray< T, N > &);
    friend bool operator!= <> (const GArray < T, N > &, const GArray< T, N > &);


    /**
     *  GArray to string serialization.
     */
    friend std::ostream& operator<< <> (std::ostream &, const GArray< T, N > &);

};




/**
 *  Forward declaration for GVector< T, N >::transform.
 */
template < typename T, std::size_t N >
class GMatrix;




/**
 *  Base class for all Vector types.
 */
template < typename T, std::size_t N >
class GVector : public GArray< T, N > {

public:

    /**
     *  Compute square length of the current vector.
     */
    T sqr_length () const {
        T result = static_cast<T>(0);
        for (std::size_t i = 0;  i < N;  i++) {
            result += sqr((*this)[i]);
        }
        return result;
    }


    /**
     *  Compute length of the current vector.
     */
    T length () const {
        return std::sqrt(sqr_length());
    }


    /**
     *  Perform a vector normalization.
     */
    GVector< T, N >& normalize () {
        T l = length();
        if (close_to(l, static_cast<T>(0))) {
            this->reset();
        } else {
            scale(static_cast<T>(1) / length());
        }
        return *this;
    }


    /**
     *  Is it a unit vector?
     */
    bool is_unit () const {
        return close_to(length(), static_cast<T>(1));
    }


    /**
     *  Is this vector length equal to zero?
     */
    bool is_zero () const {
        return close_to(length(), static_cast<T>(0));
    }


    /**
     *  Turn vector in opposite direction.
     */
    GVector< T, N >& flip () {
        for (std::size_t i = 0;  i < N;  i++) {
            GArray< T, N >::data[i] = -GArray< T, N>::data[i];
        }
        return *this;
    }


    /**
     *  Compute dot product based on current vector and one passed in parameter.
     */
    T dot (const GVector< T, N > &v) const {
        T result = static_cast<T>(0);
        for (std::size_t i = 0;  i < N;  i++) {
            result += (*this)[i] * v[i];
        }
        return result;
    }


    /**
     *  Transform given vector by a matrix and store the result in current vector.
     */
    GVector< T, N >& transform (const GMatrix< T, N > &m, const GVector< T, N > &v) {
        T val;
        for (std::size_t i = 0;  i < N;  i++) {
            val = static_cast<T>(0);
            for (std::size_t j = 0;  j < N;  j++) {
                val += m[i + j*N] * v[j];
            }
            GArray< T, N >::data[i] = val;
        }
        return *this;
    }

};




/**
 *  Vector3D specialization.
 */
template < typename T >
class GVector3 : public GVector< T, 3 > {

public:

    GVector3 () {}
    GVector3 (const GArray< T, 3 > &v) { GVector< T, 3 >::assign(*v); }
    GVector3 (const T d[]) { GVector< T, 3 >::assign(d); }
    GVector3 (T x0, T x1, T x2) { assign(x0, x1, x2); }


    GVector3< T >& operator= (const GArray< T, 3 > &v) {
        return static_cast< GVector3< T >& >(GVector< T, 3 >::assign(*v));
    }
    GVector3< T >& operator= (const T d[]) {
        return static_cast< GVector3< T >& >(GVector< T, 3 >::assign(d));
    }


    GVector3< T >& assign (T x0, T x1, T x2) {
        #define set(i, v) GVector< T, 3 >::data[i] = v
        set(0, x0); set(1, x1); set(2, x2);
        #undef set
        return *this;
    }


    /**
     *  Compute cross product and store the result in current vector.
     */
    GVector3< T >& cross (const GVector3< T > &u, const GVector3< T > &v) {
        GVector< T, 3 >::data[0] = u[1] * v[2] - v[1] * u[2];
        GVector< T, 3 >::data[1] = u[2] * v[0] - v[2] * u[0];
        GVector< T, 3 >::data[2] = u[0] * v[1] - v[0] * u[1];
        return *this;
    }

};




/**
 *  Four component vector specialization.
 */
template < typename T >
class GVector4 : public GVector< T, 4 > {

public:

    GVector4 () {}
    GVector4 (const GArray< T, 4 > &v) { GVector< T, 4 >::assign(*v); }
    GVector4 (const T d[]) { GVector< T, 4 >::assign(d); }
    GVector4 (T x0, T x1, T x2, T x3) { assign(x0, x1, x2, x3); }


    GVector4< T >& operator= (const GArray< T, 4 > &v) {
        return static_cast< GVector4< T >& >(GVector< T, 4 >::assign(*v));
    }
    GVector4< T >& operator= (const T d[]) {
        return static_cast< GVector4< T >& >(GVector< T, 4 >::assign(d));
    }


    GVector4< T >& assign (T x0, T x1, T x2, T x3) {
        #define set(i, v) GVector< T, 4 >::data[i] = v
        set(0, x0); set(1, x1); set(2, x2); set(3, x3);
        #undef set
        return *this;
    }

};




/**
 *  Base class for all matrix types.
 */
template < typename T, std::size_t N >
class GMatrix : public GArray< T, N*N > {

public:

    /**
     *  Replace matrix with identity.
     */
    GMatrix< T, N >& load_identity () {
        this->reset();
        for (std::size_t i = 0;  i < N*N;  i += (N+1)) {
            GArray< T, N*N >::data[i] = static_cast<T>(1);
        }
        return *this;
    }


    /**
     *  Multiply two matrices and store the result in the current matrix.
     */
    GMatrix< T, N >& multiply (GMatrix< T, N > &l, GMatrix< T, N > &r) {
        T val;
        std::size_t row, column;
        for (std::size_t i = 0;  i < N*N;  i++) {
            val = static_cast<T>(0);
            row = i % N;  column = (i / N) * N;
            for (std::size_t j = 0;  j < N;  j++) {
                val += l[row + j*N] * r[column + j];
            }
            GArray< T, N*N >::data[i] = val;
        }
        return *this;
    }

};




/**
 *  4x4 column-major matrix specialization.
 */
template < typename T >
class GMatrix4x4 : public GMatrix< T, 4 > {

public:

    GMatrix4x4 () { }
    GMatrix4x4 (const GArray< T, 4*4 > &m) { GMatrix< T, 4 >::assign(*m); }
    GMatrix4x4 (const T d[]) { GMatrix<T, 4>::assign(d); }


    GMatrix4x4< T >& operator= (const GArray< T, 4*4 > &m) {
        return static_cast< GMatrix4x4< T >& >(GMatrix< T, 4 >::assign(*m));
    }
    GMatrix4x4< T >& operator= (const T d[]) {
        return static_cast< GMatrix4x4< T >& >(GMatrix<T, 4>::assign(d));
    }


    /**
     *  Replaces current matrix with the translation matrix.
     */
    GMatrix4x4< T >& load_translation (T x, T y, T z) {
        this->load_identity();
        GMatrix< T, 4 >::data[12] = x;
        GMatrix< T, 4 >::data[13] = y;
        GMatrix< T, 4 >::data[14] = z;
        return *this;
    }


    /**
     *  Replaces current matrix with the rotation matrix.
     */
    GMatrix4x4< T >& load_rotation (T angle, T x, T y, T z) {
        angle = deg_to_rad(angle);
        GVector3< T > v(x, y, z);
        v.normalize();

        #define X (v[0])
        #define Y (v[1])
        #define Z (v[2])

        T
             c = std::cos(angle),
            nc = static_cast<T>(1)-c,
             s = std::sin(angle),
            xs = X*s,       ys = Y*s,       zs = Z*s,
          xync = X*Y*nc,  xznc = X*Z*nc,  yznc = Y*Z*nc;

        #define set(i, val) GMatrix< T, 4 >::data[i] = static_cast<T>(val)

        set(0, sqr(X)*nc+c);  set(4, xync-zs);      set(8 , xznc+ys);      set(12, 0);
        set(1, xync+zs);      set(5, sqr(Y)*nc+c);  set(9 , yznc-xs);      set(13, 0);
        set(2, xznc-ys);      set(6, yznc+xs);      set(10, sqr(Z)*nc+c);  set(14, 0);
        set(3, 0);            set(7, 0);            set(11, 0);            set(15, 1);

        #undef set
        #undef Z
        #undef Y
        #undef X

        return *this;
    }


    /**
     *  Replaces current matrix with the perspective projection matrix.
     */
    GMatrix4x4< T >& load_perspective (T fovy, T aspect, T zNear, T zFar) {
        fovy = deg_to_rad(fovy);
        T   f = static_cast<T>(1) / (std::tan(fovy * static_cast<T>(0.5))),
            zNear_zFar = zNear - zFar;

        GMatrix< T, 4 >::data[0] = f / aspect;
        GMatrix< T, 4 >::data[5] = f;
        GMatrix< T, 4 >::data[10] = (zNear + zFar) / zNear_zFar;
        GMatrix< T, 4 >::data[11] = static_cast<T>(-1);
        GMatrix< T, 4 >::data[14] = (static_cast<T>(2) * zNear * zFar) / zNear_zFar;

        return *this;
    }

};




} // namespace m3d

#endif
