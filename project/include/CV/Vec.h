#ifndef CVRANGEFINDER_VEC_H
#define CVRANGEFINDER_VEC_H

#include "Matx.h"

template<typename _Tp, int cn>
class Vec : public Matx<_Tp, cn, 1> {
public:
    typedef _Tp value_type;
    enum {
        channels = cn,
        _dummy_enum_finalizer = 0
    };

    //! default constructor
    Vec();

    Vec(_Tp v0); //!< 1-element vector constructor
    Vec(_Tp v0, _Tp v1); //!< 2-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2); //!< 3-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 4-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 5-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 6-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 7-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 8-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 9-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 10-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13); //!< 14-element vector constructor
    explicit Vec(const _Tp* values);

    Vec(std::initializer_list<_Tp>);

    Vec(const Vec<_Tp, cn>& v);

    static Vec all(_Tp alpha);
    static Vec ones();
    static Vec randn(_Tp a, _Tp b);
    static Vec randu(_Tp a, _Tp b);
    static Vec zeros();


    //! per-element multiplication
    Vec mul(const Vec<_Tp, cn>& v) const;

    //! conjugation (makes sense for complex numbers and quaternions)
    Vec conj() const;

    /*!
      cross product of the two 3D vectors.

      For other dimensionalities the exception is raised
    */
    Vec cross(const Vec& v) const;
    //! conversion to another data type
    template<typename T2> operator Vec<T2, cn>() const;

    /*! element access */
    const _Tp& operator [](int i) const;
    _Tp& operator[](int i);
    const _Tp& operator ()(int i) const;
    _Tp& operator ()(int i);

    Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_AddOp);
    Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_SubOp);
    template<typename _T2> Vec(const Matx<_Tp, cn, 1>& a, _T2 alpha, Matx_ScaleOp);
};

/////////////////////////////////// Vec Implementation ///////////////////////////////////

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec() {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0)
        : Matx<_Tp, cn, 1>(v0) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1)
        : Matx<_Tp, cn, 1>(v0, v1) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2)
        : Matx<_Tp, cn, 1>(v0, v1, v2) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7, v8) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13)
        : Matx<_Tp, cn, 1>(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(const _Tp* values)
        : Matx<_Tp, cn, 1>(values) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(std::initializer_list<_Tp> list)
        : Matx<_Tp, cn, 1>(list) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(const Vec<_Tp, cn>& m)
        : Matx<_Tp, cn, 1>(m.val) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_AddOp op)
        : Matx<_Tp, cn, 1>(a, b, op) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn>::Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_SubOp op)
        : Matx<_Tp, cn, 1>(a, b, op) {}

template<typename _Tp, int cn> template<typename _T2> inline
Vec<_Tp, cn>::Vec(const Matx<_Tp, cn, 1>& a, _T2 alpha, Matx_ScaleOp op)
        : Matx<_Tp, cn, 1>(a, alpha, op) {}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> Vec<_Tp, cn>::all(_Tp alpha)
{
    Vec v;
    for( int i = 0; i < cn; i++ ) v.val[i] = alpha;
    return v;
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> Vec<_Tp, cn>::ones()
{
    return Vec::all(1);
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> Vec<_Tp, cn>::zeros()
{
    return Vec::all(0);
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> Vec<_Tp, cn>::mul(const Vec<_Tp, cn>& v) const
{
    Vec<_Tp, cn> w;
    for( int i = 0; i < cn; i++ ) w.val[i] = saturate_cast<_Tp>(this->val[i]*v.val[i]);
    return w;
}

template<> inline
Vec<float, 2> Vec<float, 2>::conj() const
{
    return cv::internal::conjugate(*this);
}

template<> inline
Vec<double, 2> Vec<double, 2>::conj() const
{
    return cv::internal::conjugate(*this);
}

template<> inline
Vec<float, 4> Vec<float, 4>::conj() const
{
    return cv::internal::conjugate(*this);
}

template<> inline
Vec<double, 4> Vec<double, 4>::conj() const
{
    return cv::internal::conjugate(*this);
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> Vec<_Tp, cn>::cross(const Vec<_Tp, cn>&) const
{
    CV_StaticAssert(cn == 3, "for arbitrary-size vector there is no cross-product defined");
    return Vec<_Tp, cn>();
}

template<> inline
Vec<float, 3> Vec<float, 3>::cross(const Vec<float, 3>& v) const
{
    return Vec<float,3>(this->val[1]*v.val[2] - this->val[2]*v.val[1],
                        this->val[2]*v.val[0] - this->val[0]*v.val[2],
                        this->val[0]*v.val[1] - this->val[1]*v.val[0]);
}

template<> inline
Vec<double, 3> Vec<double, 3>::cross(const Vec<double, 3>& v) const
{
    return Vec<double,3>(this->val[1]*v.val[2] - this->val[2]*v.val[1],
                         this->val[2]*v.val[0] - this->val[0]*v.val[2],
                         this->val[0]*v.val[1] - this->val[1]*v.val[0]);
}

template<typename _Tp, int cn> template<typename T2> inline
Vec<_Tp, cn>::operator Vec<T2, cn>() const
{
    Vec<T2, cn> v;
    for( int i = 0; i < cn; i++ ) v.val[i] = saturate_cast<T2>(this->val[i]);
    return v;
}

template<typename _Tp, int cn> inline
const _Tp& Vec<_Tp, cn>::operator [](int i) const
{
    CV_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline
_Tp& Vec<_Tp, cn>::operator [](int i)
{
    CV_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline
const _Tp& Vec<_Tp, cn>::operator ()(int i) const
{
    CV_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline
_Tp& Vec<_Tp, cn>::operator ()(int i)
{
    CV_DbgAssert( (unsigned)i < (unsigned)cn );
    return this->val[i];
}

template<typename _Tp, int cn> inline
Vec<_Tp, cn> normalize(const Vec<_Tp, cn>& v)
{
    double nv = norm(v);
    return v * (nv ? 1./nv : 0.);
}

/** @name Shorter aliases for the most popular specializations of Vec<T,n>
  @{
*/
typedef Vec<uchar, 2> Vec2b;
typedef Vec<uchar, 3> Vec3b;
typedef Vec<uchar, 4> Vec4b;

typedef Vec<short, 2> Vec2s;
typedef Vec<short, 3> Vec3s;
typedef Vec<short, 4> Vec4s;

typedef Vec<ushort, 2> Vec2w;
typedef Vec<ushort, 3> Vec3w;
typedef Vec<ushort, 4> Vec4w;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;
typedef Vec<int, 6> Vec6i;
typedef Vec<int, 8> Vec8i;

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;
typedef Vec<float, 6> Vec6f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;
typedef Vec<double, 6> Vec6d;



#endif //CVRANGEFINDER_VEC_H
