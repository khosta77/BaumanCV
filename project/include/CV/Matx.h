#ifndef CVRANGEFINDER_MATX_H
#define CVRANGEFINDER_MATX_H

#include "uchar_in_typedef.h"

template<typename _Tp, int m, int n>
class Matx {
public:
    enum {
        rows     = m,
        cols     = n,
        channels = rows*cols,
//#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
//        depth    = traits::Type<_Tp>::value,
//           type     = CV_MAKETYPE(depth, channels),
//#endif
        shortdim = (m < n ? m : n)
    };

    typedef _Tp                    value_type;
    typedef Matx<_Tp, m, n>        mat_type;
    typedef Matx<_Tp, shortdim, 1> diag_type;

    //! default constructor
    Matx();

    explicit Matx(_Tp v0); //!< 1x1 matrix
    Matx(_Tp v0, _Tp v1); //!< 1x2 or 2x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2); //!< 1x3 or 3x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 1x4, 2x2 or 4x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 1x5 or 5x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 1x6, 2x3, 3x2 or 6x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 1x7 or 7x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 1x8, 2x4, 4x2 or 8x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 1x9, 3x3 or 9x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 1x10, 2x5 or 5x2 or 10x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11); //!< 1x12, 2x6, 3x4, 4x3, 6x2 or 12x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11,
         _Tp v12, _Tp v13); //!< 1x14, 2x7, 7x2 or 14x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11,
         _Tp v12, _Tp v13, _Tp v14, _Tp v15); //!< 1x16, 4x4 or 16x1 matrix
    explicit Matx(const _Tp* vals); //!< initialize from a plain array

    Matx(std::initializer_list<_Tp>); //!< initialize from an initializer list

    static Matx all(_Tp alpha);
    static Matx zeros();
    static Matx ones();
    static Matx eye();
    static Matx diag(const diag_type& d);
    /** @brief Generates uniformly distributed random numbers
    @param a Range boundary.
    @param b The other range boundary (boundaries don't have to be ordered, the lower boundary is inclusive,
    the upper one is exclusive).
     */
     static Matx randu(_Tp a, _Tp b);
    /** @brief Generates normally distributed random numbers
    @param a Mean value.
    @param b Standard deviation.
     */
     static Matx randn(_Tp a, _Tp b);

    //! dot product computed with the default precision
    _Tp dot(const Matx<_Tp, m, n>& v) const;

    //! dot product computed in double-precision arithmetics
    double ddot(const Matx<_Tp, m, n>& v) const;

    //! conversion to another data type
    template<typename T2> operator Matx<T2, m, n>() const;

    //! change the matrix shape
    template<int m1, int n1> Matx<_Tp, m1, n1> reshape() const;

    //! extract part of the matrix
    template<int m1, int n1> Matx<_Tp, m1, n1> get_minor(int base_row, int base_col) const;

    //! extract the matrix row
    Matx<_Tp, 1, n> row(int i) const;

    //! extract the matrix column
    Matx<_Tp, m, 1> col(int i) const;

    //! extract the matrix diagonal
    diag_type diag() const;

    //! transpose the matrix
    Matx<_Tp, n, m> t() const;

    //! invert the matrix
//    Matx<_Tp, n, m> inv(int method=DECOMP_LU, bool *p_is_ok = NULL) const;

    //! solve linear system
    template<int l> Matx<_Tp, n, l> solve(const Matx<_Tp, m, l>& rhs, int flags=DECOMP_LU) const;
//    Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const;

    //! multiply two matrices element-wise
    Matx<_Tp, m, n> mul(const Matx<_Tp, m, n>& a) const;

    //! divide two matrices element-wise
    Matx<_Tp, m, n> div(const Matx<_Tp, m, n>& a) const;

    //! element access
    const _Tp& operator ()(int row, int col) const;
    _Tp& operator ()(int row, int col);

    //! 1D element access
    const _Tp& operator ()(int i) const;
    _Tp& operator ()(int i);

//    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_AddOp);
//    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_SubOp);
//    template<typename _T2> Matx(const Matx<_Tp, m, n>& a, _T2 alpha, Matx_ScaleOp);
//    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_MulOp);
//    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_DivOp);
//    template<int l> Matx(const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b, Matx_MatMulOp);
//    Matx(const Matx<_Tp, n, m>& a, Matx_TOp);

    _Tp val[m*n]; //< matrix elements
};

////////////////////////////// Matx methods depending on core API /////////////////////////////

namespace internal
{

    template<typename _Tp, int m, int n> struct Matx_FastInvOp
    {
        bool operator()(const Matx<_Tp, m, n>& a, Matx<_Tp, n, m>& b, int method) const
        {
            return invert(a, b, method) != 0;
        }
    };

//    template<typename _Tp, int m> struct Matx_FastInvOp<_Tp, m, m>
//    {
//        bool operator()(const Matx<_Tp, m, m>& a, Matx<_Tp, m, m>& b, int method) const
//        {
//            if (method == DECOMP_LU || method == DECOMP_CHOLESKY)
//            {
//                Matx<_Tp, m, m> temp = a;
//
//                // assume that b is all 0's on input => make it a unity matrix
//                for (int i = 0; i < m; i++)
//                    b(i, i) = (_Tp)1;
//
//                if (method == DECOMP_CHOLESKY)
//                    return Cholesky(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m);
//
//                return LU(temp.val, m*sizeof(_Tp), m, b.val, m*sizeof(_Tp), m) != 0;
//            }
//            else
//            {
//                return invert(a, b, method) != 0;
//            }
//        }
//    };

    template<typename _Tp> struct Matx_FastInvOp<_Tp, 2, 2>
    {
        bool operator()(const Matx<_Tp, 2, 2>& a, Matx<_Tp, 2, 2>& b, int /*method*/) const
        {
            _Tp d = (_Tp)determinant(a);
            if (d == 0)
                return false;
            d = 1/d;
            b(1,1) = a(0,0)*d;
            b(0,0) = a(1,1)*d;
            b(0,1) = -a(0,1)*d;
            b(1,0) = -a(1,0)*d;
            return true;
        }
    };

    template<typename _Tp> struct Matx_FastInvOp<_Tp, 3, 3>
    {
        bool operator()(const Matx<_Tp, 3, 3>& a, Matx<_Tp, 3, 3>& b, int /*method*/) const
        {
            _Tp d = (_Tp)determinant(a);
            if (d == 0)
                return false;
            d = 1/d;
            b(0,0) = (a(1,1) * a(2,2) - a(1,2) * a(2,1)) * d;
            b(0,1) = (a(0,2) * a(2,1) - a(0,1) * a(2,2)) * d;
            b(0,2) = (a(0,1) * a(1,2) - a(0,2) * a(1,1)) * d;

            b(1,0) = (a(1,2) * a(2,0) - a(1,0) * a(2,2)) * d;
            b(1,1) = (a(0,0) * a(2,2) - a(0,2) * a(2,0)) * d;
            b(1,2) = (a(0,2) * a(1,0) - a(0,0) * a(1,2)) * d;

            b(2,0) = (a(1,0) * a(2,1) - a(1,1) * a(2,0)) * d;
            b(2,1) = (a(0,1) * a(2,0) - a(0,0) * a(2,1)) * d;
            b(2,2) = (a(0,0) * a(1,1) - a(0,1) * a(1,0)) * d;
            return true;
        }
    };


    template<typename _Tp, int m, int l, int n> struct Matx_FastSolveOp
    {
        bool operator()(const Matx<_Tp, m, l>& a, const Matx<_Tp, m, n>& b,
                        Matx<_Tp, l, n>& x, int method) const
        {
            return cv::solve(a, b, x, method);
        }
    };

//    template<typename _Tp, int m, int n> struct Matx_FastSolveOp<_Tp, m, m, n>
//    {
//        bool operator()(const Matx<_Tp, m, m>& a, const Matx<_Tp, m, n>& b,
//                        Matx<_Tp, m, n>& x, int method) const
//        {
//            if (method == DECOMP_LU || method == DECOMP_CHOLESKY)
//            {
//                Matx<_Tp, m, m> temp = a;
//                x = b;
//                if( method == DECOMP_CHOLESKY )
//                    return Cholesky(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n);
//
//                return LU(temp.val, m*sizeof(_Tp), m, x.val, n*sizeof(_Tp), n) != 0;
//            }
//            else
//            {
//                return cv::solve(a, b, x, method);
//            }
//        }
//    };

//    template<typename _Tp> struct Matx_FastSolveOp<_Tp, 2, 2, 1>
//    {
//        bool operator()(const Matx<_Tp, 2, 2>& a, const Matx<_Tp, 2, 1>& b,
//                        Matx<_Tp, 2, 1>& x, int) const
//        {
//            _Tp d = (_Tp)determinant(a);
//            if (d == 0)
//                return false;
//            d = 1/d;
//            x(0) = (b(0)*a(1,1) - b(1)*a(0,1))*d;
//            x(1) = (b(1)*a(0,0) - b(0)*a(1,0))*d;
//            return true;
//        }
//    };

    template<typename _Tp> struct Matx_FastSolveOp<_Tp, 3, 3, 1>
    {
        bool operator()(const Matx<_Tp, 3, 3>& a, const Matx<_Tp, 3, 1>& b,
                        Matx<_Tp, 3, 1>& x, int) const
        {
            _Tp d = (_Tp)determinant(a);
            if (d == 0)
                return false;
            d = 1/d;
            x(0) = d*(b(0)*(a(1,1)*a(2,2) - a(1,2)*a(2,1)) -
                      a(0,1)*(b(1)*a(2,2) - a(1,2)*b(2)) +
                      a(0,2)*(b(1)*a(2,1) - a(1,1)*b(2)));

            x(1) = d*(a(0,0)*(b(1)*a(2,2) - a(1,2)*b(2)) -
                      b(0)*(a(1,0)*a(2,2) - a(1,2)*a(2,0)) +
                      a(0,2)*(a(1,0)*b(2) - b(1)*a(2,0)));

            x(2) = d*(a(0,0)*(a(1,1)*b(2) - b(1)*a(2,1)) -
                      a(0,1)*(a(1,0)*b(2) - b(1)*a(2,0)) +
                      b(0)*(a(1,0)*a(2,1) - a(1,1)*a(2,0)));
            return true;
        }
    };

} // internal

//template<typename _Tp, int m, int n> inline
//Matx<_Tp,m,n> Matx<_Tp,m,n>::randu(_Tp a, _Tp b)
//{
//    Matx<_Tp,m,n> M;
//    cv::randu(M, Scalar(a), Scalar(b));
//    return M;
//}
//
//template<typename _Tp, int m, int n> inline
//Matx<_Tp,m,n> Matx<_Tp,m,n>::randn(_Tp a, _Tp b)
//{
//    Matx<_Tp,m,n> M;
//    cv::randn(M, Scalar(a), Scalar(b));
//    return M;
//}
//
//template<typename _Tp, int cn> inline
//Vec<_Tp, cn> Vec<_Tp, cn>::randu(_Tp a, _Tp b)
//{
//    Vec<_Tp,cn> V;
//    cv::randu(V, Scalar(a), Scalar(b));
//    return V;
//}
//
//template<typename _Tp, int cn> inline
//Vec<_Tp, cn> Vec<_Tp, cn>::randn(_Tp a, _Tp b)
//{
//    Vec<_Tp,cn> V;
//    cv::randn(V, Scalar(a), Scalar(b));
//    return V;
//}
//
//template<typename _Tp, int m, int n> inline
//Matx<_Tp, n, m> Matx<_Tp, m, n>::inv(int method, bool *p_is_ok /*= NULL*/) const
//{
//    Matx<_Tp, n, m> b;
//    bool ok = cv::internal::Matx_FastInvOp<_Tp, m, n>()(*this, b, method);
//    if (p_is_ok) *p_is_ok = ok;
//    return ok ? b : Matx<_Tp, n, m>::zeros();
//}
//
//template<typename _Tp, int m, int n> template<int l> inline
//Matx<_Tp, n, l> Matx<_Tp, m, n>::solve(const Matx<_Tp, m, l>& rhs, int method) const
//{
//    Matx<_Tp, n, l> x;
//    bool ok = cv::internal::Matx_FastSolveOp<_Tp, m, n, l>()(*this, rhs, x, method);
//    return ok ? x : Matx<_Tp, n, l>::zeros();
//}


typedef Matx<float, 1, 2> Matx12f;
typedef Matx<double, 1, 2> Matx12d;
typedef Matx<float, 1, 3> Matx13f;
typedef Matx<double, 1, 3> Matx13d;
typedef Matx<float, 1, 4> Matx14f;
typedef Matx<double, 1, 4> Matx14d;
typedef Matx<float, 1, 6> Matx16f;
typedef Matx<double, 1, 6> Matx16d;

typedef Matx<float, 2, 1> Matx21f;
typedef Matx<double, 2, 1> Matx21d;
typedef Matx<float, 3, 1> Matx31f;
typedef Matx<double, 3, 1> Matx31d;
typedef Matx<float, 4, 1> Matx41f;
typedef Matx<double, 4, 1> Matx41d;
typedef Matx<float, 6, 1> Matx61f;
typedef Matx<double, 6, 1> Matx61d;

typedef Matx<float, 2, 2> Matx22f;
typedef Matx<double, 2, 2> Matx22d;
typedef Matx<float, 2, 3> Matx23f;
typedef Matx<double, 2, 3> Matx23d;
typedef Matx<float, 3, 2> Matx32f;
typedef Matx<double, 3, 2> Matx32d;

typedef Matx<float, 3, 3> Matx33f;
typedef Matx<double, 3, 3> Matx33d;

typedef Matx<float, 3, 4> Matx34f;
typedef Matx<double, 3, 4> Matx34d;
typedef Matx<float, 4, 3> Matx43f;
typedef Matx<double, 4, 3> Matx43d;

typedef Matx<float, 4, 4> Matx44f;
typedef Matx<double, 4, 4> Matx44d;
typedef Matx<float, 6, 6> Matx66f;
typedef Matx<double, 6, 6> Matx66d;

//struct CV_EXPORTS Matx_AddOp { Matx_AddOp() {} Matx_AddOp(const Matx_AddOp&) {} };
//struct CV_EXPORTS Matx_SubOp { Matx_SubOp() {} Matx_SubOp(const Matx_SubOp&) {} };
//struct CV_EXPORTS Matx_ScaleOp { Matx_ScaleOp() {} Matx_ScaleOp(const Matx_ScaleOp&) {} };
//struct CV_EXPORTS Matx_MulOp { Matx_MulOp() {} Matx_MulOp(const Matx_MulOp&) {} };
//struct CV_EXPORTS Matx_DivOp { Matx_DivOp() {} Matx_DivOp(const Matx_DivOp&) {} };
//struct CV_EXPORTS Matx_MatMulOp { Matx_MatMulOp() {} Matx_MatMulOp(const Matx_MatMulOp&) {} };
//struct CV_EXPORTS Matx_TOp { Matx_TOp() {} Matx_TOp(const Matx_TOp&) {} };

#endif //CVRANGEFINDER_MATX_H
