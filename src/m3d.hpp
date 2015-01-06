/**
 *  machina
 *
 *  Copyright (c) 2011, drmats
 *  All rights reserved.
 *
 *  https://github.com/drmats/machina
 */

#ifndef __M3D_HPP_
#define __M3D_HPP_ 1

#include "sdl_opengl.hpp"
#include <cstring>
#include <stdexcept>
#include <ostream>
#include <cmath>
#include "m3d_message.hpp"

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
template <typename T>
inline bool close_to (T a, T b) {
    return std::fabs(a - b) < static_cast<T>(m3d_epsilon);
}




/**
 *  Square root.
 */
template <typename T>
inline T sqr (T x) {
    return x * x;
}




/**
 *  Angle conversions.
 */
template <typename T>
inline T deg_to_rad (T x) {
    return x * static_cast<T>(m3d_pi180);
}

template <typename T>
inline T rad_to_deg (T x) {
    return x * static_cast<T>(m3d_invpi180);
}




/**
 *  Linear interpolation.
 */
template <typename T>
inline T linear_interpolation (T x1, T x2, T x3, T y1, T y3) {
    return (((x2 - x1) * (y3 - y1)) / (x3 - x1)) + y1;
}




/**
 *  Forward declarations for GArray friend operators.
 */
template <typename T, std::size_t N>
class GArray;

template <typename T, std::size_t N>
bool operator== (const GArray<T, N> &, const GArray<T, N> &);

template <typename T, std::size_t N>
bool operator!= (const GArray<T, N> &, const GArray<T, N> &);

template <typename T, std::size_t N>
std::ostream& operator<< (std::ostream &, const GArray<T, N> &);




/**
 *  Base abstract class for Points, Vectors and Matrices.
 */
template <typename T, std::size_t N>
class GArray {

protected:

    T data[N] = { 0 };


public:

    virtual ~GArray () = 0;
    inline GArray<T, N>& operator= (const GArray<T, N> &a) { return this->assign(*a); }
    inline GArray<T, N>& operator= (const T d[]) { return this->assign(d); }


    /**
     *  Array indexers.
     */
    inline T& operator[] (std::size_t i) throw (std::out_of_range) {
        if (i >= N) {
            throw std::out_of_range(m3d_message::out_of_range);
        }
        return this->data[i];
    }

    inline const T& operator[] (std::size_t i) const throw (std::out_of_range) {
        if (i >= N) {
            throw std::out_of_range(m3d_message::out_of_range);
        }
        return this->data[i];
    }


    /**
     *  Return pointer of basic type to array.
     */
    inline const T* operator* () const { return this->data; }


    /**
     *  Return size of array.
     */
    inline std::size_t size () const { return N; }


    /**
     *  Reset current array (fill data with zeros).
     */
    inline GArray<T, N>& reset () {
        std::memset(this->data, 0, N*sizeof(T));
        return *this;
    }


    /**
     *  Replace current array.
     */
    inline GArray<T, N>& assign (const GArray<T, N> &a) {
        std::memcpy(this->data, *a, N*sizeof(T));
        return *this;
    }

    inline GArray<T, N>& assign (const T d[]) {
        std::memcpy(this->data, d, N*sizeof(T));
        return *this;
    }


    /**
     *  Add to current array.
     */
    inline GArray<T, N>& add (const GArray<T, N> &a) {
        for (std::size_t i = 0;  i < N;  i++) {
            this->data[i] += a[i];
        }
        return *this;
    }


    /**
     *  Subtract from current array.
     */
    inline GArray<T, N>& sub (const GArray<T, N> &a) {
        for (std::size_t i = 0;  i < N;  i++) {
            this->data[i] -= a[i];
        }
        return *this;
    }


    /**
     *  Scale current array (multiply each component by a given value).
     */
    inline GArray<T, N>& scale (T s) {
        for (std::size_t i = 0;  i < N;  i++) {
            this->data[i] *= s;
        }
        return *this;
    }


    /**
     *  GArray deep comparision.
     */
    friend bool operator== <> (const GArray<T, N> &, const GArray<T, N> &);
    friend bool operator!= <> (const GArray<T, N> &, const GArray<T, N> &);


    /**
     *  GArray to string serialization.
     */
    friend std::ostream& operator<< <> (std::ostream &, const GArray<T, N> &);

};




/**
 *  Forward declaration for GVector<T, N>::transform.
 */
template <typename T, std::size_t N>
class GMatrix;




/**
 *  Base class for all Vector types.
 */
template <typename T, std::size_t N>
class GVector : public GArray<T, N> {

public:

    using GArray<T, N>::operator=;


    /**
     *  Compute square length of the current vector.
     */
    inline T sqr_length () const {
        T result = static_cast<T>(0);
        for (std::size_t i = 0;  i < N;  i++) {
            result += sqr((*this)[i]);
        }
        return result;
    }


    /**
     *  Compute length of the current vector.
     */
    inline T length () const {
        return std::sqrt(this->sqr_length());
    }


    /**
     *  Perform a vector normalization.
     */
    GVector<T, N>& normalize () {
        const T l = this->length();
        if (close_to(l, static_cast<T>(0))) {
            this->reset();
        } else {
            this->scale(static_cast<T>(1) / this->length());
        }
        return *this;
    }


    /**
     *  Is it a unit vector?
     */
    inline bool is_unit () const {
        return close_to(this->length(), static_cast<T>(1));
    }


    /**
     *  Is this vector length equal to zero?
     */
    inline bool is_zero () const {
        return close_to(this->length(), static_cast<T>(0));
    }


    /**
     *  Turn vector in opposite direction.
     */
    GVector<T, N>& flip () {
        for (std::size_t i = 0;  i < N;  i++) {
            this->data[i] = -this->data[i];
        }
        return *this;
    }


    /**
     *  Compute dot product based on the current vector and one passed in parameter.
     */
    T dot (const GVector<T, N> &v) const {
        T result = static_cast<T>(0);
        for (std::size_t i = 0;  i < N;  i++) {
            result += this->data[i] * v[i];
        }
        return result;
    }


    /**
     *  Transform given vector by a matrix and store the result in current vector.
     */
    GVector<T, N>& transform (const GMatrix<T, N> &m, const GVector<T, N> &v) {
        T val;
        for (std::size_t i = 0;  i < N;  i++) {
            val = static_cast<T>(0);
            for (std::size_t j = 0;  j < N;  j++) {
                val += m[i + j*N] * v[j];
            }
            this->data[i] = val;
        }
        return *this;
    }

};




/**
 *  Vector2D specialization.
 */
template <typename T>
class GVector2 : public GVector<T, 2> {

public:

    GVector2 () {}
    GVector2 (T x0, T x1) { this->assign(x0, x1); }
    using GVector<T, 2>::operator=;


    GVector2<T>& assign (T x0, T x1) {
        #define set(i, v) this->data[i] = v
        set(0, x0); set(1, x1);
        #undef set
        return *this;
    }

};




/**
 *  Vector3D specialization.
 */
template <typename T>
class GVector3 : public GVector<T, 3> {

public:

    GVector3 () {}
    GVector3 (T x0, T x1, T x2) { this->assign(x0, x1, x2); }
    using GVector<T, 3>::operator=;


    GVector3<T>& assign (T x0, T x1, T x2) {
        #define set(i, v) this->data[i] = v
        set(0, x0); set(1, x1); set(2, x2);
        #undef set
        return *this;
    }


    /**
     *  Compute cross product and store the result in the current vector.
     */
    GVector3<T>& cross (const GVector3<T> &u, const GVector3<T> &v) {
        this->data[0] = u[1] * v[2] - v[1] * u[2];
        this->data[1] = u[2] * v[0] - v[2] * u[0];
        this->data[2] = u[0] * v[1] - v[0] * u[1];
        return *this;
    }

};




/**
 *  Four component vector specialization.
 */
template <typename T>
class GVector4 : public GVector<T, 4> {

public:

    GVector4 () {}
    GVector4 (T x0, T x1, T x2, T x3) { this->assign(x0, x1, x2, x3); }
    using GVector<T, 4>::operator=;


    GVector4<T>& assign (T x0, T x1, T x2, T x3) {
        #define set(i, v) this->data[i] = v
        set(0, x0); set(1, x1); set(2, x2); set(3, x3);
        #undef set
        return *this;
    }

};




/**
 *  Base class for all matrix types.
 */
template <typename T, std::size_t N>
class GMatrix : public GArray<T, N*N> {

public:

    using GArray<T, N*N>::operator=;


    /**
     *  Replace matrix with identity.
     */
    GMatrix<T, N>& load_identity () {
        this->reset();
        for (std::size_t i = 0;  i < N*N;  i += (N+1)) {
            this->data[i] = static_cast<T>(1);
        }
        return *this;
    }


    /**
     *  Multiply two matrices and store the result in the current matrix.
     */
    GMatrix<T, N>& multiply (const GMatrix<T, N> &l, const GMatrix<T, N> &r) {
        T val;
        std::size_t row, column;
        for (std::size_t i = 0;  i < N*N;  i++) {
            val = static_cast<T>(0);
            row = i % N;  column = (i / N) * N;
            for (std::size_t j = 0;  j < N;  j++) {
                val += l[row + j*N] * r[column + j];
            }
            this->data[i] = val;
        }
        return *this;
    }

};




/**
 *  4x4 column-major matrix specialization.
 */
template <typename T>
class GMatrix4x4 : public GMatrix<T, 4> {

public:

    using GMatrix<T, 4>::operator=;


    /**
     *  Replaces current matrix with the translation matrix.
     */
    inline GMatrix4x4<T>& load_translation (T x, T y, T z) {
        this->load_identity();
        this->data[12] = x;
        this->data[13] = y;
        this->data[14] = z;
        return *this;
    }


    /**
     *  Replaces current matrix with the translation matrix (built from vector).
     */
    GMatrix4x4<T>& load_translation (const GVector3<T> &v) {
        return this->load_translation(v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the scale matrix.
     */
    inline GMatrix4x4<T>& load_scale (T x, T y, T z) {
        this->reset();
        this->data[0] = x;
        this->data[5] = y;
        this->data[10] = z;
        this->data[15] = static_cast<T>(1);
        return *this;
    }


    /**
     *  Replaces current matrix with the scale matrix (built from vector).
     */
    GMatrix4x4<T>& load_scale (const GVector3<T> &v) {
        return this->load_scale(v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the rotation matrix.
     */
    inline GMatrix4x4<T>& load_rotation (T angle, T x, T y, T z) {
        angle = deg_to_rad(angle);

        GVector3<T> v(x, y, z); v.normalize();
        const T
             X = v[0], Y = v[1], Z = v[2],
             c = std::cos(angle),
            nc = static_cast<T>(1)-c,
             s = std::sin(angle),
            xs = X*s,       ys = Y*s,       zs = Z*s,
          xync = X*Y*nc,  xznc = X*Z*nc,  yznc = Y*Z*nc;

        #define set(i, val) this->data[i] = static_cast<T>(val)

        set(0, sqr(X)*nc+c);  set(4, xync-zs);      set(8 , xznc+ys);      set(12, 0);
        set(1, xync+zs);      set(5, sqr(Y)*nc+c);  set(9 , yznc-xs);      set(13, 0);
        set(2, xznc-ys);      set(6, yznc+xs);      set(10, sqr(Z)*nc+c);  set(14, 0);
        set(3, 0);            set(7, 0);            set(11, 0);            set(15, 1);

        #undef set

        return *this;
    }


    /**
     *  Replaces current matrix with the rotation matrix (built from angle and vector).
     */
    GMatrix4x4<T>& load_rotation (T angle, const GVector3<T> &v) {
        return this->load_rotation(angle, v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the perspective projection matrix.
     */
    GMatrix4x4<T>& load_perspective (T fovy, T aspect, T z_near, T z_far) {
        this->reset();
        fovy = deg_to_rad(fovy);

        const T
            f = static_cast<T>(1) / (std::tan(fovy * static_cast<T>(0.5))),
            nf = z_near - z_far;

        this->data[0] = f / aspect;
        this->data[5] = f;
        this->data[10] = (z_near + z_far) / nf;
        this->data[11] = static_cast<T>(-1);
        this->data[14] = (static_cast<T>(2) * z_near * z_far) / nf;

        return *this;
    }


    /**
     *  Replaces current matrix with the perspective projection matrix.
     */
    GMatrix4x4<T>& load_ortho (T left, T right, T bottom, T top, T z_near, T z_far) {
        this->reset();

        const T
            rl = right - left,
            tb = top - bottom,
            fn = z_far - z_near;

        this->data[0] = static_cast<T>(2) / rl;
        this->data[5] = static_cast<T>(2) / tb;
        this->data[10] = static_cast<T>(-2) / fn;
        this->data[12] = static_cast<T>(-1) * ((right + left) / rl);
        this->data[13] = static_cast<T>(-1) * ((top + bottom) / tb);
        this->data[14] = static_cast<T>(-1) * ((z_far + z_near) / fn);
        this->data[15] = static_cast<T>(1);

        return *this;
    }

};




} // namespace m3d

#endif
