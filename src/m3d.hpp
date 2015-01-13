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
#include <initializer_list>
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
 *  Angle conversion (degrees to radians).
 */
template <typename T>
inline T radians (T x) {
    return x * static_cast<T>(m3d_pi180);
}


/**
 *  Angle conversion (radians to degrees).
 */
template <typename T>
inline T degrees (T x) {
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
 *  Computes the positive remainder of the division operation val/denom.
 */
template <typename T>
inline T positive_fmod (T val, T denom) {
    return std::fmod(std::fmod(val, denom) + denom, denom);
}


/**
 *  Base class for Points, Vectors and Matrices.
 */
template <typename T, std::size_t N>
class GArray {

protected:

    T data[N] = { 0 };


public:

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
     *  Return pointer of basic type to the internal array.
     */
    inline const T* operator* () const { return this->data; }


    /**
     *  Return the size of this array.
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


    inline GArray<T, N>& operator= (const GArray<T, N> &a) {
        return this->assign(a);
    }


    inline GArray<T, N>& operator= (const T d[]) {
        return this->assign(d);
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


    inline GArray<T, N>& operator+= (const GArray<T, N> &a) {
        return this->add(a);
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


    inline GArray<T, N>& operator-= (const GArray<T, N> &a) {
        return this->sub(a);
    }


    /**
     *  Scale the current array (multiply each component by a given value).
     */
    inline GArray<T, N>& scale (const T s) {
        for (std::size_t i = 0;  i < N;  i++) {
            this->data[i] *= s;
        }
        return *this;
    }


    inline GArray<T, N>& operator*= (const T s) {
        return this->scale(s);
    }

};


/**
 *  Add two arrays yielding the new one.
 */
template <typename T, std::size_t N>
inline GArray<T, N> operator+ (const GArray<T, N> &a, const GArray<T, N> &b) {
    return GArray<T, N>(a).add(b);
}


/**
 *  Subtract two arrays yielding the new one.
 */
template <typename T, std::size_t N>
inline GArray<T, N> operator- (const GArray<T, N> &a, const GArray<T, N> &b) {
    return GArray<T, N>(a).sub(b);
}


/**
 *  Scale array by a scalar yielding the new array (Array * Scalar).
 */
template <typename T, std::size_t N>
inline GArray<T, N> operator* (const GArray<T, N> &a, const T s) {
    return GArray<T, N>(a).scale(s);
}


/**
 *  Scale array by a scalar yielding the new array (Scalar * Array).
 */
template <typename T, std::size_t N>
inline GArray<T, N> operator* (const T s, const GArray<T, N> &a) {
    return GArray<T, N>(a).scale(s);
}


/**
 *  GArray deep comparision "==".
 */
template <typename T, std::size_t N>
inline bool operator== (const GArray<T, N> &a, const GArray<T, N> &b) {
    for (std::size_t i = 0;  i < N;  i++) {
        if (a[i] != b[i]) { return false; }
    }
    return true;
}


/**
 *  GArray deep comparision "!=".
 */
template <typename T, std::size_t N>
inline bool operator!= (const GArray<T, N> &l, const GArray<T, N> &r) {
    return !(l == r);
}


/**
 *  GArray to string serialization.
 */
template <typename T, std::size_t N>
std::ostream& operator<< (std::ostream &os, const GArray<T, N> &gv) {
    os << "[";
    for (std::size_t i = 0;  i < N;  i++) {
        os << gv[i];
        if (i < N-1) { os << ", "; }
    }
    os << "]";
    return os;
}




/**
 *  Base class for all Vector types.
 */
template <typename T, std::size_t N>
class GVector : public GArray<T, N> {

public:

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
    T dot (const GArray<T, N> &v) const {
        T result = static_cast<T>(0);
        for (std::size_t i = 0;  i < N;  i++) {
            result += this->data[i] * v[i];
        }
        return result;
    }


    /**
     *  Transform given vector by a matrix and store the result in the current vector.
     */
    GVector<T, N>& transform (const GArray<T, N*N> &m, const GArray<T, N> &v) {
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
 *  Transform given vector by a matrix and store the result in the new vector.
 */
template <typename T, std::size_t N>
inline GVector<T, N> operator* (const GArray<T, N*N> &m, const GArray<T, N> &v) {
    return GVector<T, N>().transform(m, v);
}




/**
 *  Vector2D specialization.
 */
template <typename T>
class GVector2 : public GVector<T, 2> {

public:

    GVector2 () {}
    GVector2 (T x0, T x1) { this->assign(x0, x1); }
    GVector2 (const GArray<T, 2> &a) { GArray<T, 2>::assign(a); }
    GVector2 (const T d[]) { GArray<T, 2>::assign(d); }


    using GArray<T, 2>::operator=;


    using GArray<T, 2>::assign;


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
    GVector3 (const GArray<T, 3> &a) { GArray<T, 3>::assign(a); }
    GVector3 (const GArray<T, 4> &a) { this->assign(a); }
    GVector3 (const T d[]) { GArray<T, 3>::assign(d); }


    using GArray<T, 3>::operator=;


    using GArray<T, 3>::assign;


    inline GVector3<T>& assign (const GArray<T, 4> &a) {
        return static_cast<GVector3<T>&>(this->assign(*a));
    }


    inline GVector3<T>& assign (T x0, T x1, T x2) {
        #define set(i, v) this->data[i] = v
        set(0, x0); set(1, x1); set(2, x2);
        #undef set
        return *this;
    }


    /**
     *  Compute cross product and store the result in the current vector.
     */
    GVector3<T>& cross (const GArray<T, 3> &u, const GArray<T, 3> &v) {
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
    GVector4 (const GArray<T, 3> &a, T w) { this->assign(a, w); }
    GVector4 (const GArray<T, 4> &a) { GArray<T, 4>::assign(a); }
    GVector4 (const T d[]) { GArray<T, 4>::assign(d); }


    using GArray<T, 4>::operator=;


    using GArray<T, 4>::assign;


    inline GVector4<T>& assign (T x0, T x1, T x2, T x3) {
        #define set(i, v) this->data[i] = v
        set(0, x0); set(1, x1); set(2, x2); set(3, x3);
        #undef set
        return *this;
    }


    inline GVector4<T>& assign (const GArray<T, 3> &a, T w) {
        #define set(i, v) this->data[i] = v
        set(0, a[0]); set(1, a[1]); set(2, a[2]); set(3, w);
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

    GMatrix () {}
    GMatrix (const GArray<T, N*N> &a) { GArray<T, N*N>::assign(a); }
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
    GMatrix<T, N>& multiply (const GArray<T, N*N> &a, const GArray<T, N*N> &b) {
        T val;
        std::size_t row, column;
        for (std::size_t i = 0;  i < N*N;  i++) {
            val = static_cast<T>(0);
            row = i % N;  column = (i / N) * N;
            for (std::size_t j = 0;  j < N;  j++) {
                val += a[row + j*N] * b[column + j];
            }
            this->data[i] = val;
        }
        return *this;
    }

};


/**
 *  Multiply two matrices yielding the new one.
 */
template <typename T, std::size_t N = 4>
inline GMatrix<T, N> operator* (const GArray<T, N*N> &a, const GArray<T, N*N> &b) {
    return GMatrix<T, N>().multiply(a, b);
}




/**
 *  4x4 column-major matrix specialization.
 */
template <typename T>
class GMatrix4 : public GMatrix<T, 4> {

public:

    GMatrix4 () {}


    GMatrix4 (std::initializer_list<T> init_list) {
        typename std::initializer_list<T>::iterator it;
        std::size_t i;
        for (
            it = init_list.begin(), i = 0;
            it != init_list.end()  &&  i < 4*4;
            it++, i++
        ) {
            this->data[i] = *it;
        }
    }


    GMatrix4 (const GArray<T, 4*4> &a) { GArray<T, 4*4>::assign(a); }


    using GArray<T, 4*4>::operator=;


    /**
     *  Replaces current matrix with the translation matrix.
     */
    inline GMatrix4<T>& load_translation (T x, T y, T z) {
        this->load_identity();
        #define set(i, v) this->data[i] = v
        set(12, x); set(13, y); set(14, z);
        #undef set
        return *this;
    }


    /**
     *  Replaces current matrix with the translation matrix (built from vector).
     */
    GMatrix4<T>& load_translation (const GArray<T, 3> &v) {
        return this->load_translation(v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the scale matrix.
     */
    inline GMatrix4<T>& load_scale (T x, T y, T z) {
        this->reset();
        #define set(i, v) this->data[i] = v
        set(0, x); set(5, y); set(10, z); set(15, static_cast<T>(1));
        #undef set
        return *this;
    }


    /**
     *  Replaces current matrix with the scale matrix (built from vector).
     */
    GMatrix4<T>& load_scale (const GArray<T, 3> &v) {
        return this->load_scale(v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the rotation matrix.
     */
    inline GMatrix4<T>& load_rotation (T angle, T x, T y, T z) {
        angle = radians(angle);

        GVector3<T> v(x, y, z); v.normalize();
        const T
             X = v[0], Y = v[1], Z = v[2],
             c = std::cos(angle),
            nc = static_cast<T>(1)-c,
             s = std::sin(angle),
            xs = X*s,       ys = Y*s,       zs = Z*s,
          xync = X*Y*nc,  xznc = X*Z*nc,  yznc = Y*Z*nc;

        #define set(i, val) this->data[i] = static_cast<T>(val)

        set(0, sqr(X)*nc+c);  set(4,     xync-zs);  set( 8,     xznc+ys);  set(12, 0);
        set(1,     xync+zs);  set(5, sqr(Y)*nc+c);  set( 9,     yznc-xs);  set(13, 0);
        set(2,     xznc-ys);  set(6,     yznc+xs);  set(10, sqr(Z)*nc+c);  set(14, 0);
        set(3,           0);  set(7,           0);  set(11,           0);  set(15, 1);

        #undef set

        return *this;
    }


    /**
     *  Replaces current matrix with the rotation matrix (built from angle and vector).
     */
    GMatrix4<T>& load_rotation (T angle, const GArray<T, 3> &v) {
        return this->load_rotation(angle, v[0], v[1], v[2]);
    }


    /**
     *  Replaces current matrix with the perspective projection matrix.
     */
    GMatrix4<T>& load_perspective (T fovy, T aspect, T z_near, T z_far) {
        this->reset();
        fovy = radians(fovy);

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
    GMatrix4<T>& load_ortho (T left, T right, T bottom, T top, T z_near, T z_far) {
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
