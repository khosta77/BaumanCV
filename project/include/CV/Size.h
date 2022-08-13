#ifndef CVRANGEFINDER_SIZE_H
#define CVRANGEFINDER_SIZE_H

#include "iostream"

template<typename _Tp> class Size_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Size_() : width(0), height(0) {}
    Size_(_Tp _width, _Tp _height) : width(_width), height(_height) {} // !!!
//    Size_(const Point_<_Tp>& pt);
    //! the area (width*height)
    _Tp area() const;
    //! aspect ratio (width/height)
    double aspectRatio() const;
    //! true if empty
    bool empty() const;

    //! conversion of another data type.
    template<typename _Tp2> operator Size_<_Tp2>() const;

    _Tp width; //!< the width
    _Tp height; //!< the height
};

//template<typename _Tp> inline
//Size_<_Tp>::Size_() : width(0), height(0) {}
//
//template<typename _Tp> inline
//Size_<_Tp>::Size_(_Tp _width, _Tp _height) : width(_width), height(_height) {}

//template<typename _Tp> inline
//Size_<_Tp>::Size_(const Point_<_Tp>& pt)
//        : width(pt.x), height(pt.y) {}

//template<typename _Tp> template<typename _Tp2> inline
//Size_<_Tp>::operator Size_<_Tp2>() const
//{
//    return Size_<_Tp2>(saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height));
//}

template<typename _Tp> inline
_Tp Size_<_Tp>::area() const
{
    const _Tp result = width * height;
//    CV_DbgAssert(!std::numeric_limits<_Tp>::is_integer
//                 || width == 0 || result / width == height); // make sure the result fits in the return value
    return result;
}

template<typename _Tp> inline
double Size_<_Tp>::aspectRatio() const
{
    return width / static_cast<double>(height);
}

template<typename _Tp> inline
bool Size_<_Tp>::empty() const
{
    return width <= 0 || height <= 0;
}


template<typename _Tp> static inline
Size_<_Tp>& operator *= (Size_<_Tp>& a, _Tp b)
{
    a.width *= b;
    a.height *= b;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator * (const Size_<_Tp>& a, _Tp b)
{
    Size_<_Tp> tmp(a);
    tmp *= b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator /= (Size_<_Tp>& a, _Tp b)
{
    a.width /= b;
    a.height /= b;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator / (const Size_<_Tp>& a, _Tp b)
{
    Size_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator += (Size_<_Tp>& a, const Size_<_Tp>& b)
{
    a.width += b.width;
    a.height += b.height;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator + (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    Size_<_Tp> tmp(a);
    tmp += b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator -= (Size_<_Tp>& a, const Size_<_Tp>& b)
{
    a.width -= b.width;
    a.height -= b.height;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator - (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    Size_<_Tp> tmp(a);
    tmp -= b;
    return tmp;
}

template<typename _Tp> static inline
bool operator == (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    return a.width == b.width && a.height == b.height;
}

template<typename _Tp> static inline
bool operator != (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    return !(a == b);
}

typedef Size_<int> Size2i;
typedef Size2i Size;

struct MatSize {
    explicit MatSize(int *_p);
    int dims() const;
    Size operator()() const;
    const int &operator[](int i) const;
    int &operator[](int i);
    operator const int *() const;
    bool operator==(const MatSize &sz) const;
    bool operator!=(const MatSize &sz) const;
    int *p;
};

inline
MatSize::MatSize(int* _p)
: p(_p) {}

inline
int MatSize::dims() const
{
return (p - 1)[0];
}

inline
Size MatSize::operator()() const
{
//    CV_DbgAssert(dims() <= 2);
    return Size(p[1], p[0]);
}

inline
const int& MatSize::operator[](int i) const
{
//    CV_DbgAssert(i < dims());
//#ifdef __OPENCV_BUILD
//    CV_DbgAssert(i >= 0);
//#endif
    return p[i];
}

inline
int& MatSize::operator[](int i)
{
//    CV_DbgAssert(i < dims());
//#ifdef __OPENCV_BUILD
//    CV_DbgAssert(i >= 0);
//#endif
    return p[i];
}

inline
MatSize::operator const int*() const
{
return p;
}

inline
bool MatSize::operator != (const MatSize& sz) const
{
return !(*this == sz);
}

struct /*CV_EXPORTS*/ MatStep
{
    MatStep() ;
    explicit MatStep(size_t s) ;
    const size_t& operator[](int i) const ;
    size_t& operator[](int i) ;
    operator size_t() const;
    MatStep& operator = (size_t s);

    size_t* p;
    size_t buf[2];
protected:
    MatStep& operator = (const MatStep&);
};

inline
MatStep::MatStep()
{
p = buf; p[0] = p[1] = 0;
}

inline
MatStep::MatStep(size_t s)
{
p = buf; p[0] = s; p[1] = 0;
}

inline
const size_t& MatStep::operator[](int i) const
{
return p[i];
}

inline
size_t& MatStep::operator[](int i)
{
return p[i];
}

inline MatStep::operator size_t() const
{
//    CV_DbgAssert( p == buf );
    return buf[0];
}

inline MatStep& MatStep::operator = (size_t s)
{
//    CV_DbgAssert( p == buf );
    buf[0] = s;
    return *this;
}

#endif //CVRANGEFINDER_SIZE_H
