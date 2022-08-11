//
// Created by stepan on 11.08.22.
//

#ifndef CVRANGEFINDER_MAT_H
#define CVRANGEFINDER_MAT_H

#include "ptr.h"
#include "algorithm"

class  Mat {
public:
    Mat();

    Mat(int rows, int cols, int type);

    Mat(Size size, int type);


    Mat(int rows, int cols, int type, const Scalar &s);


    Mat(Size size, int type, const Scalar &s);


    Mat(int ndims, const int *sizes, int type);


    Mat(const std::vector<int> &sizes, int type);


    Mat(int ndims, const int *sizes, int type, const Scalar &s);


    Mat(const std::vector<int> &sizes, int type, const Scalar &s);


    Mat(const Mat &m);

    Mat(int rows, int cols, int type, void *data, size_t step = AUTO_STEP);


    Mat(Size size, int type, void *data, size_t step = AUTO_STEP);

    Mat(const std::vector<int> &sizes, int type, void *data, const size_t *steps = 0);

    Mat(const Mat &m, const Range &rowRange, const Range &colRange = Range::all());


    Mat(const Mat &m, const Rect &roi);


    Mat(const Mat &m, const Range *ranges);


    Mat(const Mat &m, const std::vector <Range> &ranges);


    template<typename _Tp>
    explicit Mat(const std::vector <_Tp> &vec, bool copyData = false);

    /** @overload
    */
    template<typename _Tp, typename = typename std::enable_if<std::is_arithmetic<_Tp>::value>::type>
    explicit Mat(const std::initializer_list <_Tp> list);

    /** @overload
    */
    template<typename _Tp>
    explicit Mat(const std::initializer_list<int> sizes, const std::initializer_list <_Tp> list);

    /** @overload
    */
    template<typename _Tp, size_t _Nm>
    explicit Mat(const std::array <_Tp, _Nm> &arr, bool copyData = false);

    /** @overload
    */
    template<typename _Tp, int n>
    explicit Mat(const Vec <_Tp, n> &vec, bool copyData = true);

    /** @overload
    */
    template<typename _Tp, int m, int n>
    explicit Mat(const Matx <_Tp, m, n> &mtx, bool copyData = true);


    template<typename _Tp>
    explicit Mat(const Point_ <_Tp> &pt, bool copyData = true);

    template<typename _Tp>
    explicit Mat(const Point3_ <_Tp> &pt, bool copyData = true);


    template<typename _Tp>
    explicit Mat(const MatCommaInitializer_ <_Tp> &commaInitializer);

    explicit Mat(const cuda::GpuMat &m);

    ~Mat();


    Mat &operator=(const Mat &m);


    Mat &operator=(const MatExpr &expr);

    //! retrieve UMat from Mat
    UMat getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags = USAGE_DEFAULT) const;


    Mat row(int y) const;


    Mat col(int x) const;


    Mat rowRange(int startrow, int endrow) const;


    Mat rowRange(const Range &r) const;


    Mat colRange(int startcol, int endcol) const;


    Mat colRange(const Range &r) const;


    Mat diag(int d = 0) const;


    CV_NODISCARD_STD static Mat

    diag(const Mat &d);


    CV_NODISCARD_STD Mat

    clone() const;


    void copyTo(OutputArray m) const;


    void copyTo(OutputArray m, InputArray mask) const;


    void convertTo(OutputArray m, int rtype, double alpha = 1, double beta = 0) const;


    Mat &operator=(const Scalar &s);


    Mat &setTo(InputArray value, InputArray mask = noArray());


    Mat reshape(int cn, int rows = 0) const;

    /** @overload */
    Mat reshape(int cn, int newndims, const int *newsz) const;

    /** @overload */
    Mat reshape(int cn, const std::vector<int> &newshape) const;


    MatExpr t() const;


    MatExpr inv(int method = DECOMP_LU) const;


    MatExpr mul(InputArray m, double scale = 1) const;


    Mat cross(InputArray m) const;


    double dot(InputArray m) const;

    CV_NODISCARD_STD static MatExpr

    zeros(int rows, int cols, int type);


    CV_NODISCARD_STD static MatExpr zeros(Size size, int type );


    CV_NODISCARD_STD static MatExpr

    zeros(int ndims, const int *sz, int type);


    CV_NODISCARD_STD static MatExpr

    ones(int rows, int cols, int type);


    CV_NODISCARD_STD static MatExpr ones(Size  size, int type);


    CV_NODISCARD_STD static MatExpr

    ones(int ndims, const int *sz, int type);


    CV_NODISCARD_STD static MatExpr

    eye(int rows, int cols, int type);


    CV_NODISCARD_STD static MatExpr eye(Size size, int type);


    void create(int rows, int cols, int type);

    /** @overload
    @param size Alternative new matrix size specification: Size(cols, rows)
    @param type New matrix type.
    */
    void create(Size size, int type);

    /** @overload
    @param ndims New array dimensionality.
    @param sizes Array of integers specifying a new array shape.
    @param type New matrix type.
    */
    void create(int ndims, const int *sizes, int type);

    /** @overload
    @param sizes Array of integers specifying a new array shape.
    @param type New matrix type.
    */
    void create(const std::vector<int> &sizes, int type);


    void addref();


    void release();

    //! internal use function, consider to use 'release' method instead; deallocates the matrix data
    void deallocate();

    //! internal use function; properly re-allocates _size, _step arrays
    void copySize(const Mat &m);

    /** @brief Reserves space for the certain number of rows.

    The method reserves space for sz rows. If the matrix already has enough space to store sz rows,
    nothing happens. If the matrix is reallocated, the first Mat::rows rows are preserved. The method
    emulates the corresponding method of the STL vector class.
    @param sz Number of rows.
     */
    void reserve(size_t sz);

    /** @brief Reserves space for the certain number of bytes.

    The method reserves space for sz bytes. If the matrix already has enough space to store sz bytes,
    nothing happens. If matrix has to be reallocated its previous content could be lost.
    @param sz Number of bytes.
    */
    void reserveBuffer(size_t sz);

    /** @brief Changes the number of matrix rows.

    The methods change the number of matrix rows. If the matrix is reallocated, the first
    min(Mat::rows, sz) rows are preserved. The methods emulate the corresponding methods of the STL
    vector class.
    @param sz New number of rows.
     */
    void resize(size_t sz);

    /** @overload
    @param sz New number of rows.
    @param s Value assigned to the newly added elements.
     */
    void resize(size_t sz, const Scalar &s);

    //! internal function
    void push_back_(const void *elem);


    template<typename _Tp>
    void push_back(const _Tp &elem);

    /** @overload
    @param elem Added element(s).
    */
    template<typename _Tp>
    void push_back(const Mat_ <_Tp> &elem);


    template<typename _Tp>
    void push_back(const std::vector <_Tp> &elem);


    void push_back(const Mat &m);

    void pop_back(size_t nelems = 1);


    void locateROI(Size &wholeSize, Point &ofs) const;


    Mat &adjustROI(int dtop, int dbottom, int dleft, int dright);


    Mat operator()(Range rowRange, Range colRange) const;


    Mat operator()(const Rect &roi) const;


    Mat operator()(const std::vector <Range> &ranges) const;

    template<typename _Tp>
    operator std::vector<_Tp>() const;

    template<typename _Tp, int n>
    operator Vec<_Tp, n>() const;

    template<typename _Tp, int m, int n>
    operator Matx<_Tp, m, n>() const;

    template<typename _Tp, std::size_t _Nm>
    operator std::array<_Tp, _Nm>() const;

    bool isContinuous() const;

    //! returns true if the matrix is a submatrix of another matrix
    bool isSubmatrix() const;

    size_t elemSize() const;
    size_t elemSize1() const;


    int type() const;
    int depth() const;
    int channels() const;

    size_t step1(int i = 0) const;
    bool empty() const;
    size_t total() const;
    size_t total(int startDim, int endDim = INT_MAX) const;

    uchar *ptr(int i0 = 0);

    /** @overload */
    const uchar *ptr(int i0 = 0) const;


    uchar *ptr(int row, int col);


    const uchar *ptr(int row, int col) const;

    /** @overload */
    uchar *ptr(int i0, int i1, int i2);

    /** @overload */
    const uchar *ptr(int i0, int i1, int i2) const;

    /** @overload */
    uchar *ptr(const int *idx);

    /** @overload */
    const uchar *ptr(const int *idx) const;

    /** @overload */
    template<int n>
    uchar *ptr(const Vec<int, n> &idx);

    /** @overload */
    template<int n>
    const uchar *ptr(const Vec<int, n> &idx) const;

    /** @overload */
    template<typename _Tp>
    _Tp *ptr(int i0 = 0);

    /** @overload */
    template<typename _Tp>
    const _Tp *ptr(int i0 = 0) const;


    template<typename _Tp>
    _Tp *ptr(int row, int col);


    template<typename _Tp>
    const _Tp *ptr(int row, int col) const;

    /** @overload */
    template<typename _Tp>
    _Tp *ptr(int i0, int i1, int i2);

    /** @overload */
    template<typename _Tp>
    const _Tp *ptr(int i0, int i1, int i2) const;

    /** @overload */
    template<typename _Tp>
    _Tp *ptr(const int *idx);

    /** @overload */
    template<typename _Tp>
    const _Tp *ptr(const int *idx) const;

    /** @overload */
    template<typename _Tp, int n>
    _Tp *ptr(const Vec<int, n> &idx);

    /** @overload */
    template<typename _Tp, int n>
    const _Tp *ptr(const Vec<int, n> &idx) const;

    template<typename _Tp>
    _Tp &at(int i0 = 0);

    /** @overload
    @param i0 Index along the dimension 0
    */
    template<typename _Tp>
    const _Tp &at(int i0 = 0) const;

    /** @overload
    @param row Index along the dimension 0
    @param col Index along the dimension 1
    */
    template<typename _Tp>
    _Tp &at(int row, int col);

    /** @overload
    @param row Index along the dimension 0
    @param col Index along the dimension 1
    */
    template<typename _Tp>
    const _Tp &at(int row, int col) const;

    /** @overload
    @param i0 Index along the dimension 0
    @param i1 Index along the dimension 1
    @param i2 Index along the dimension 2
    */
    template<typename _Tp>
    _Tp &at(int i0, int i1, int i2);

    /** @overload
    @param i0 Index along the dimension 0
    @param i1 Index along the dimension 1
    @param i2 Index along the dimension 2
    */
    template<typename _Tp>
    const _Tp &at(int i0, int i1, int i2) const;

    /** @overload
    @param idx Array of Mat::dims indices.
    */
    template<typename _Tp>
    _Tp &at(const int *idx);

    /** @overload
    @param idx Array of Mat::dims indices.
    */
    template<typename _Tp>
    const _Tp &at(const int *idx) const;

    /** @overload */
    template<typename _Tp, int n>
    _Tp &at(const Vec<int, n> &idx);

    /** @overload */
    template<typename _Tp, int n>
    const _Tp &at(const Vec<int, n> &idx) const;

    /** @overload
    special versions for 2D arrays (especially convenient for referencing image pixels)
    @param pt Element position specified as Point(j,i) .
    */
    template<typename _Tp>
    _Tp &at(Point pt);


    template<typename _Tp>
    const _Tp &at(Point pt) const;

    template<typename _Tp>
    MatIterator_ <_Tp> begin();

    template<typename _Tp>
    MatConstIterator_ <_Tp> begin() const;

    /** @brief Same as begin() but for inverse traversal
     */
    template<typename _Tp>
    std::reverse_iterator <MatIterator_<_Tp>> rbegin();

    template<typename _Tp>
    std::reverse_iterator <MatConstIterator_<_Tp>> rbegin() const;


    template<typename _Tp>
    MatIterator_ <_Tp> end();

    template<typename _Tp>
    MatConstIterator_ <_Tp> end() const;

    /** @brief Same as end() but for inverse traversal
     */
    template<typename _Tp>
    std::reverse_iterator <MatIterator_<_Tp>> rend();

    template<typename _Tp>
    std::reverse_iterator <MatConstIterator_<_Tp>> rend() const;


    template<typename _Tp, typename Functor>
    void forEach(const Functor &operation);

    /** @overload */
    template<typename _Tp, typename Functor>
    void forEach(const Functor &operation) const;

    Mat(Mat &&m);

    Mat &operator=(Mat &&m);

    enum {
        MAGIC_VAL = 0x42FF0000,
        AUTO_STEP = 0,
        CONTINUOUS_FLAG = CV_MAT_CONT_FLAG,
        SUBMATRIX_FLAG = CV_SUBMAT_FLAG
    };
    enum {
        MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7
    };

    /*! includes several bit-fields:
         - the magic signature
         - continuity flag
         - depth
         - number of channels
     */
    int flags;
    //! the matrix dimensionality, >= 2
    int dims;
    //! the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
    int rows, cols;
    //! pointer to the data
    uchar *data;

    //! helper fields used in locateROI and adjustROI
    const uchar *datastart;
    const uchar *dataend;
    const uchar *datalimit;

    //! custom allocator
    MatAllocator *allocator;

    //! and the standard allocator
    static MatAllocator *getStdAllocator();

    static MatAllocator *getDefaultAllocator();

    static void setDefaultAllocator(MatAllocator *allocator);

    //! internal use method: updates the continuity flag
    void updateContinuityFlag();

    //! interaction with UMat
    UMatData *u;

    MatSize size;
    MatStep step;

protected:
    template<typename _Tp, typename Functor>
    void forEach_impl(const Functor &operation);
};


#endif //CVRANGEFINDER_MAT_H
