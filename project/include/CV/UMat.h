#ifndef CVRANGEFINDER_UMAT_H
#define CVRANGEFINDER_UMAT_H

#include "vector"
#include "Scalar.h"
#include "Size.h"

class Range;
class Rect;

class UMat {
public:
    //! default constructor
    UMat(UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! constructs 2D matrix of the specified size and type
    // (_type is CV_8UC1, CV_64FC3, CV_32SC(12) etc.)
    UMat(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! constructs 2D matrix and fills it with the specified value _s.
    UMat(int rows, int cols, int type, const Scalar &s, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(Size size, int type, const Scalar &s, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! constructs n-dimensional matrix
    UMat(int ndims, const int *sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(int ndims, const int *sizes, int type, const Scalar &s, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! copy constructor
    UMat(const UMat &m);

    //! creates a matrix header for a part of the bigger matrix
    UMat(const UMat &m, const Range &rowRange, const Range &colRange = Range::all());

    UMat(const UMat &m, const Rect &roi);

    UMat(const UMat &m, const Range *ranges);

    UMat(const UMat &m, const std::vector <Range> &ranges);

    // FIXIT copyData=false is not implemented, drop this in favor of cv::Mat (OpenCV 5.0)
    //! builds matrix from std::vector with or without copying the data
    template<typename _Tp>
    explicit UMat(const std::vector <_Tp> &vec, bool copyData = false);

    //! destructor - calls release()
    ~UMat();

    //! assignment operators
    UMat &operator=(const UMat &m);

    Mat getMat(AccessFlag flags) const;

    //! returns a new matrix header for the specified row
    UMat row(int y) const;

    //! returns a new matrix header for the specified column
    UMat col(int x) const;

    //! ... for the specified row span
    UMat rowRange(int startrow, int endrow) const;

    UMat rowRange(const Range &r) const;

    //! ... for the specified column span
    UMat colRange(int startcol, int endcol) const;

    UMat colRange(const Range &r) const;

    //! ... for the specified diagonal
    //! (d=0 - the main diagonal,
    //!  >0 - a diagonal from the upper half,
    //!  <0 - a diagonal from the lower half)
    UMat diag(int d = 0) const;
    //! constructs a square diagonal matrix which main diagonal is vector "d"
//                CV_NODISCARD_STD static UMat diag(const UMat& d, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat diag(const UMat& d) { return diag(d, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//
//                //! returns deep copy of the matrix, i.e. the data is copied
//                CV_NODISCARD_STD UMat clone() const;
    //! copies the matrix content to "m".
    // It calls m.create(this->size(), this->type()).
//                void copyTo( OutputArray m ) const;
//                //! copies those matrix elements to "m" that are marked with non-zero mask elements.
//                void copyTo( OutputArray m, InputArray mask ) const;
//                //! converts matrix to another datatype with optional scaling. See cvConvertScale.
//                void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;

    void assignTo(UMat &m, int type = -1) const;

    //! sets every matrix element to s
    UMat &operator=(const Scalar &s);

    //! sets some of the matrix elements to s, according to the mask
//                UMat& setTo(InputArray value, InputArray mask=noArray());
    //! creates alternative matrix header for the same data, with different
    // number of channels and/or different number of rows. see cvReshape.
    UMat reshape(int cn, int rows = 0) const;

    UMat reshape(int cn, int newndims, const int *newsz) const;

    //! matrix transposition by means of matrix expressions
    UMat t() const;
    //! matrix inversion by means of matrix expressions
//                UMat inv(int method=DECOMP_LU) const;
    //! per-element matrix multiplication by means of matrix expressions
//                UMat mul(InputArray m, double scale=1) const;

    //! computes dot-product
//                double dot(InputArray m) const;

    //! Matlab-style matrix initialization
//                CV_NODISCARD_STD static UMat zeros(int rows, int cols, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat zeros(Size size, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat zeros(int ndims, const int* sz, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat zeros(int rows, int cols, int type) { return zeros(rows, cols, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat zeros(Size size, int type) { return zeros(size, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat zeros(int ndims, const int* sz, int type) { return zeros(ndims, sz, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat ones(int rows, int cols, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat ones(Size size, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat ones(int ndims, const int* sz, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat ones(int rows, int cols, int type) { return ones(rows, cols, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat ones(Size size, int type) { return ones(size, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat ones(int ndims, const int* sz, int type) { return ones(ndims, sz, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat eye(int rows, int cols, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat eye(Size size, int type, UMatUsageFlags usageFlags /*= USAGE_DEFAULT*/);
//                CV_NODISCARD_STD static UMat eye(int rows, int cols, int type) { return eye(rows, cols, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload
//                CV_NODISCARD_STD static UMat eye(Size size, int type) { return eye(size, type, USAGE_DEFAULT); }  // OpenCV 5.0: remove abi compatibility overload

    //! allocates new matrix data unless the matrix already has specified size and type.
    // previous data is unreferenced if needed.
    void create(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    void create(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    void create(int ndims, const int *sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    void create(const std::vector<int> &sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! increases the reference counter; use with care to avoid memleaks
    void addref();

    //! decreases reference counter;
    // deallocates the data when reference counter reaches 0.
    void release();

    //! deallocates the matrix data
    void deallocate();

    //! internal use function; properly re-allocates _size, _step arrays
    void copySize(const UMat &m);

    //! locates matrix header within a parent matrix. See below
    void locateROI(Size &wholeSize, Point &ofs) const;

    //! moves/resizes the current matrix ROI inside the parent matrix.
    UMat &adjustROI(int dtop, int dbottom, int dleft, int dright);

    //! extracts a rectangular sub-matrix
    // (this is a generalized form of row, rowRange etc.)
    UMat operator()(Range rowRange, Range colRange) const;

    UMat operator()(const Rect &roi) const;

    UMat operator()(const Range *ranges) const;

    UMat operator()(const std::vector <Range> &ranges) const;

    //! returns true iff the matrix data is continuous
    // (i.e. when there are no gaps between successive rows).
    // similar to CV_IS_MAT_CONT(cvmat->type)
    bool isContinuous() const;

    //! returns true if the matrix is a submatrix of another matrix
    bool isSubmatrix() const;

    //! returns element size in bytes,
    // similar to CV_ELEM_SIZE(cvmat->type)
    size_t elemSize() const;

    //! returns the size of element channel in bytes.
    size_t elemSize1() const;

    //! returns element type, similar to CV_MAT_TYPE(cvmat->type)
    int type() const;

    //! returns element type, similar to CV_MAT_DEPTH(cvmat->type)
    int depth() const;

    //! returns element type, similar to CV_MAT_CN(cvmat->type)
    int channels() const;

    //! returns step/elemSize1()
    size_t step1(int i = 0) const;

    //! returns true if matrix data is NULL
    bool empty() const;

    //! returns the total number of matrix elements
    size_t total() const;

    //! returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
    int checkVector(int elemChannels, int depth = -1, bool requireContinuous = true) const;

    UMat(UMat &&m);

    UMat &operator=(UMat &&m);

    /*! Returns the OpenCL buffer handle on which UMat operates on.
        The UMat instance should be kept alive during the use of the handle to prevent the buffer to be
        returned to the OpenCV buffer pool.
     */
    void *handle(AccessFlag accessFlags) const;

    void ndoffset(size_t *ofs) const;

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

    //! number of rows in the matrix; -1 when the matrix has more than 2 dimensions
    int rows;

    //! number of columns in the matrix; -1 when the matrix has more than 2 dimensions
    int cols;

    //! custom allocator
    MatAllocator *allocator;

    //! usage flags for allocator; recommend do not set directly, instead set during construct/create/getUMat
    UMatUsageFlags usageFlags;

    //! and the standard allocator
    static MatAllocator *getStdAllocator();

    //! internal use method: updates the continuity flag
    void updateContinuityFlag();

    //! black-box container of UMat data
    UMatData *u;

    //! offset of the submatrix (or 0)
    size_t offset;

    //! dimensional size of the matrix; accessible in various formats
    MatSize size;

    //! number of bytes each matrix element/row/plane/dimension occupies
    MatStep step;

protected:
};

#endif //CVRANGEFINDER_UMAT_H
