#ifndef CVRANGEFINDER_MAT_H
#define CVRANGEFINDER_MAT_H

#include "Vec.h"
#include "Scalar.h"
#include <climits>
#include <algorithm>
#include "Size_validateInputImageSize.h"

struct UMatData {
    enum MemoryFlag {
        COPY_ON_MAP = 1, HOST_COPY_OBSOLETE = 2,
        DEVICE_COPY_OBSOLETE = 4, TEMP_UMAT = 8, TEMP_COPIED_UMAT = 24,
        USER_ALLOCATED = 32, DEVICE_MEM_MAPPED = 64,
        ASYNC_CLEANUP = 128
    };

    UMatData(const MatAllocator *allocator);

    ~UMatData();

    // provide atomic access to the structure
    void lock();

    void unlock();

    bool hostCopyObsolete() const;

    bool deviceCopyObsolete() const;

    bool deviceMemMapped() const;

    bool copyOnMap() const;

    bool tempUMat() const;

    bool tempCopiedUMat() const;

    void markHostCopyObsolete(bool flag);

    void markDeviceCopyObsolete(bool flag);

    void markDeviceMemMapped(bool flag);

    const MatAllocator *prevAllocator;
    const MatAllocator *currAllocator;
    int urefcount;
    int refcount;
    uchar *data;
    uchar *origdata;
    size_t size;

    UMatData::MemoryFlag flags;
    void *handle;
    void *userdata;
    int allocatorFlags_;
    int mapcount;
    UMatData *originalUMatData;
    std::shared_ptr<void> allocatorContext;
};

// forward decls, implementation is below in this file
void setSize(UMat& m, int _dims, const int* _sz, const size_t* _steps,
             bool autoSteps = false);

void updateContinuityFlag(UMat& m);
void finalizeHdr(UMat& m);

UMatData::UMatData(const MatAllocator* allocator)
{
    prevAllocator = currAllocator = allocator;
    urefcount = refcount = mapcount = 0;
    data = origdata = 0;
    size = 0;
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    originalUMatData = NULL;
}

UMatData::~UMatData()
{
    prevAllocator = currAllocator = 0;
    urefcount = refcount = 0;
    CV_Assert(mapcount == 0);
    data = origdata = 0;
    size = 0;
    bool isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
    flags = static_cast<UMatData::MemoryFlag>(0);
    handle = 0;
    userdata = 0;
    allocatorFlags_ = 0;
    if (originalUMatData)
    {
        bool showWarn = false;
        UMatData* u = originalUMatData;
        bool zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
        if (zero_Ref)
        {
            // simulate Mat::deallocate
            if (u->mapcount != 0)
            {
                (u->currAllocator ? u->currAllocator : /* TODO allocator ? allocator :*/ Mat::getDefaultAllocator())->unmap(u);
            }
            else
            {
                // we don't do "map", so we can't do "unmap"
            }
        }
        bool zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
        if (zero_Ref && !zero_URef)
            showWarn = true;
        if (zero_Ref && zero_URef) // oops, we need to free resources
        {
            showWarn = !isAsyncCleanup;
            // simulate UMat::deallocate
            u->currAllocator->deallocate(u);
        }

        originalUMatData = NULL;
    }
}

void UMatData::lock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

void UMatData::unlock()
{
    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
}

//UMatDataAutoLock::UMatDataAutoLock(UMatData* u) : u1(u), u2(NULL)
//{
//    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
//}
//UMatDataAutoLock::UMatDataAutoLock(UMatData* u1_, UMatData* u2_) : u1(u1_), u2(u2_)
//{
//    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
//}
//UMatDataAutoLock::~UMatDataAutoLock()
//{
//    // nothing in OPENCV_DISABLE_THREAD_SUPPORT mode
//}

//CV_ENUM_FLAGS(UMatData::MemoryFlag)
//
//
//struct CV_EXPORTS MatSize
//        {
//                explicit MatSize(int* _p) CV_NOEXCEPT;
//                int dims() const CV_NOEXCEPT;
//                Size operator()() const;
//                const int& operator[](int i) const;
//                int& operator[](int i);
//                operator const int*() const CV_NOEXCEPT;  // TODO OpenCV 4.0: drop this
//                bool operator == (const MatSize& sz) const CV_NOEXCEPT;
//                bool operator != (const MatSize& sz) const CV_NOEXCEPT;
//
//                int* p;
//        };
//
//struct CV_EXPORTS MatStep
//        {
//                MatStep() CV_NOEXCEPT;
//        explicit MatStep(size_t s) CV_NOEXCEPT;
//        const size_t& operator[](int i) const CV_NOEXCEPT;
//        size_t& operator[](int i) CV_NOEXCEPT;
//        operator size_t() const;
//        MatStep& operator = (size_t s);
//
//        size_t* p;
//        size_t buf[2];
//        protected:
//        MatStep& operator = (const MatStep&);
//        };



class  Mat {
public:
    Mat() : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0), datalimit(0),
            allocator(0), u(0), size(&rows), step(0) {}
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
    static Mat diag(const Mat &d);
    Mat clone() const;
    void copyTo(OutputArray m) const;
    void copyTo(OutputArray m, InputArray mask) const;
    void convertTo(OutputArray m, int rtype, double alpha = 1, double beta = 0) const;
    Mat &operator=(const Scalar &s);
    Mat &setTo(InputArray value, InputArray mask = noArray());
    Mat reshape(int cn, int rows = 0) const;
    Mat reshape(int cn, int newndims, const int *newsz) const;
    Mat reshape(int cn, const std::vector<int> &newshape) const;


//    MatExpr t() const;
//    MatExpr inv(int method = DECOMP_LU) const;
//    MatExpr mul(InputArray m, double scale = 1) const;

//    Mat cross(InputArray m) const;

//    double dot(InputArray m) const;
//    static MatExpr zeros(int rows, int cols, int type);
//    static MatExpr zeros(Size size, int type );
//    static MatExpr zeros(int ndims, const int *sz, int type);
//    static MatExpr ones(int rows, int cols, int type);
//    static MatExpr ones(Size  size, int type);
//    static MatExpr ones(int ndims, const int *sz, int type);
//    static MatExpr eye(int rows, int cols, int type);
//    static MatExpr eye(Size size, int type);


    void create(int rows, int cols, int type) {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    }
    void create(Size size, int type);
    void create(int ndims, const int *sizes, int type);
    void create(const std::vector<int> &sizes, int type);


    void addref();


    void release() {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if( u && CV_XADD(&u->refcount, -1) == 1 )
                deallocate();
            u = NULL;
            datastart = dataend = datalimit = data = 0;
            for(int i = 0; i < dims; i++)
                size.p[i] = 0;
    }

    //! internal use function, consider to use 'release' method instead; deallocates the matrix data
    void deallocate() {
        if(u)
        {
            UMatData* u_ = u;
            u = NULL;
            (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
        }
    }

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

/////////////////////////////////////////////////////// MAt //////////////////////////////////////////////////


Mat::Mat() : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
             datalimit(0), allocator(0), u(0), size(&rows), step(0) {}

void Mat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type);
}

//void Mat::create(Size _sz, int _type)
//{
//    create(_sz.height, _sz.width, _type);
//}

void Mat::create(int d, const int* _sizes, int _type)
{
    int i;
//    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if ( dims == 1 && (d == 1 && _sizes[0] == size[0]) )
            return;
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    int _sizes_backup[CV_MAX_DIM]; // #5991
    if (_sizes == (this->size.p))
    {
        for(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
        if(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        catch (...)
        {
            if (a == a0)
                throw;
            u = a0->allocate(dims, size, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
}

void Mat::create(const std::vector<int>& _sizes, int _type)
{
    create((int)_sizes.size(), _sizes.data(), _type);
}

#endif //CVRANGEFINDER_MAT_H
