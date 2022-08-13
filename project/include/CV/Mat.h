#ifndef CVRANGEFINDER_MAT_H
#define CVRANGEFINDER_MAT_H

#include <memory>
#include "Vec.h"
#include "Scalar.h"
#include <climits>
#include <algorithm>
#include "Size_validateInputImageSize.h"

class Mat;
class MatAllocator;

void* fastMalloc(size_t size);
void finalizeHdr(Mat& m);
void setSize( Mat& m, int _dims, const int* _sz, const size_t* _steps, bool autoSteps=false );
static MatAllocator*& getDefaultAllocatorMatRef();
void fastFree(void* ptr);

enum AccessFlag { ACCESS_READ=1<<24, ACCESS_WRITE=1<<25,
    ACCESS_RW=3<<24, ACCESS_MASK=ACCESS_RW, ACCESS_FAST=1<<26 };

enum UMatUsageFlags {
    USAGE_DEFAULT = 0,

    // политика распределения буфера зависит от платформы и использования
    USAGE_ALLOCATE_HOST_MEMORY = 1 << 0,
    USAGE_ALLOCATE_DEVICE_MEMORY = 1 << 1,
    USAGE_ALLOCATE_SHARED_MEMORY = 1 << 2, // Это не равносильно: USAGE_ALLOCATE_HOST_MEMORY | USAGE_ALLOCATE_DEVICE_MEMORY

    __UMAT_USAGE_FLAGS_32BIT = 0x7fffffff // Подсказка о совместимости с двоичными файлами
};

class Mat;
struct UMatData {
    enum MemoryFlag {
        COPY_ON_MAP = 1, HOST_COPY_OBSOLETE = 2,
        DEVICE_COPY_OBSOLETE = 4, TEMP_UMAT = 8, TEMP_COPIED_UMAT = 24,
        USER_ALLOCATED = 32, DEVICE_MEM_MAPPED = 64,
        ASYNC_CLEANUP = 128
    };

    UMatData(const MatAllocator *allocator) {
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

    ~UMatData() {
//        prevAllocator = currAllocator = 0;
//        urefcount = refcount = 0;
//        data = origdata = 0;
//        size = 0;
//        bool isAsyncCleanup = !!(flags & UMatData::ASYNC_CLEANUP);
//        flags = static_cast<UMatData::MemoryFlag>(0);
//        handle = 0;
//        userdata = 0;
//        allocatorFlags_ = 0;
//        if (originalUMatData) {
//            bool showWarn = false;
//            UMatData* u = originalUMatData;
//            bool zero_Ref = CV_XADD(&(u->refcount), -1) == 1;
//            if (zero_Ref) {
//                // имитировать Mat::deallocate
//                if (u->mapcount != 0) {
//                    (u->currAllocator ? u->currAllocator : getDefaultAllocatorMatRef())->unmap(u);
//                } else {
//                    // мы не делаем "map", поэтому мы не можем сделать "unmap".
//                }
//            }
//            bool zero_URef = CV_XADD(&(u->urefcount), -1) == 1;
//            if (zero_Ref && !zero_URef)
//                showWarn = true;
//            if (zero_Ref && zero_URef) { // упс, нам нужно освободить ресурсы
//                showWarn = !isAsyncCleanup;
//                // имитировать UMat::deallocate
//                u->currAllocator->deallocate(u);
//            }
//
//            originalUMatData = NULL;
//        }
    }

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

class MatAllocator {
public:
    MatAllocator() {}
    virtual ~MatAllocator() {}
    virtual UMatData *allocate(int dims, const int *sizes, int type, void *data, size_t *step,
                               AccessFlag flags, UMatUsageFlags usageFlags) const = 0;
    virtual bool allocate(UMatData *data, AccessFlag accessflags, UMatUsageFlags usageFlags) const = 0;
    virtual void deallocate(UMatData *data) const = 0;
    virtual void map(UMatData *data, AccessFlag accessflags) const;
    virtual void unmap(UMatData *data) const {
        if(data->urefcount == 0 && data->refcount == 0) {
            deallocate(data);
        }
    }
    virtual void download(UMatData *data, void *dst, int dims, const size_t sz[], const size_t srcofs[],
                          const size_t srcstep[], const size_t dststep[]) const;
    virtual void upload(UMatData *data, const void *src, int dims, const size_t sz[], const size_t dstofs[],
                        const size_t dststep[], const size_t srcstep[]) const;
    virtual void copy(UMatData *srcdata, UMatData *dstdata, int dims, const size_t sz[], const size_t srcofs[],
                      const size_t srcstep[], const size_t dstofs[], const size_t dststep[], bool sync) const;
//    // default implementation returns DummyBufferPoolController
//    virtual BufferPoolController *getBufferPoolController(const char *id = NULL) const;
};

class StdMatAllocator : public MatAllocator {
public:
    UMatData *allocate(int dims, const int *sizes, int type,
                       void *data0, size_t *step, AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) const {
        size_t total = CV_ELEM_SIZE(type);
        for (int i = dims - 1; i >= 0; i--) {
            if (step) {
                if (data0 && step[i] != CV_AUTOSTEP) {
//                    CV_Assert(total <= step[i]);
                    total = step[i];
                } else
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar *data = data0 ? (uchar *) data0 : (uchar *) fastMalloc(total);
        UMatData *u = new UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
//        if (data0)
//            u->flags = u->flags | UMatData::MemoryFlag::USER_ALLOCATED;

        return u;
    }

    bool allocate(UMatData *u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const {
        if (!u) return false;
        return true;
    }

    void deallocate(UMatData *u) const {
        if (!u)
            return;

//        CV_Assert(u->urefcount == 0);
//        CV_Assert(u->refcount == 0);
        if (!(u->flags & 32)) { // USER_ALLOCATED
            fastFree(u->origdata);
            u->origdata = 0;
        }
        delete u;
    }
};

MatAllocator*& getDefaultAllocatorMatRef();
int cv_updateContinuityFlag(int flags, int dims, const int* size, const size_t* step);

class  Mat {
public:
    Mat() : flags(0x42FF0000), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0), datalimit(0),
            allocator(0), u(0), size(&rows), step(0) {}
    ~Mat();


    Mat &operator=(const Mat &m) {
        if( this != &m )
        {
            if( m.u )
                CV_XADD(&m.u->refcount, 1);
            release();
            flags = m.flags;
            if( dims <= 2 && m.dims <= 2 )
            {
                dims = m.dims;
                rows = m.rows;
                cols = m.cols;
                step[0] = m.step[0];
                step[1] = m.step[1];
            }
            else
                copySize(m);
            data = m.data;
            datastart = m.datastart;
            dataend = m.dataend;
            datalimit = m.datalimit;
            allocator = m.allocator;
            u = m.u;
        }
        return *this;
    }
//    Mat &operator=(const MatExpr &expr);

    //! retrieve UMat from Mat
//    UMat getUMat(AccessFlag accessFlags, UMatUsageFlags usageFlags = USAGE_DEFAULT) const;
//    Mat row(int y) const;
//    Mat col(int x) const;
//    Mat rowRange(int startrow, int endrow) const;
//    Mat rowRange(const Range &r) const;
//    Mat colRange(int startcol, int endcol) const;
//    Mat colRange(const Range &r) const;
//    Mat diag(int d = 0) const;
//    static Mat diag(const Mat &d);
//    Mat clone() const;
//    void copyTo(OutputArray m) const;
//    void copyTo(OutputArray m, InputArray mask) const;
//    void convertTo(OutputArray m, int rtype, double alpha = 1, double beta = 0) const;
//    Mat &operator=(const Scalar &s);  //
//    Mat &setTo(InputArray value, InputArray mask = noArray());
//    Mat reshape(int cn, int rows = 0) const;
//    Mat reshape(int cn, int newndims, const int *newsz) const;
//    Mat reshape(int cn, const std::vector<int> &newshape) const;


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

    void create(int _rows, int _cols, int _type) {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        _type &= 0x00000FFF;
        if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
            return;
        int sz[] = {_rows, _cols};
        create(2, sz, _type);
    }
//    void create(Size size, int type);
    void create(int d, const int *_sizes, int _type) {// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
        flags = (_type & CV_MAT_TYPE_MASK) | 0x42FF0000; /// ????
        setSize(*this, d, _sizes, 0, true);

        if( total() > 0 )
        {
            MatAllocator *a = allocator, *a0 = getDefaultAllocator();
            if(!a)
                a = a0;
            try
            {
                u = a->allocate(dims, size, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
//                CV_Assert(u != 0);
            }
            catch (...)
            {
                if (a == a0)
                    throw;
                u = a0->allocate(dims, size, _type, 0, step.p, ACCESS_RW /* ignored */, USAGE_DEFAULT);
//                CV_Assert(u != 0);
            }
//            CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
        }

        addref();
        finalizeHdr(*this);
    }

//    void create(const std::vector<int> &sizes, int type) {
//        create((int)_sizes.size(), _sizes.data(), _type);
//    }

    void addref() {
        if( u )
            CV_XADD(&u->refcount, 1);
    }


    void release() {  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if( u && CV_XADD(&u->refcount, -1) == 1 )
                deallocate();
            u = NULL;
            datastart = dataend = datalimit = data = 0;
            for(int i = 0; i < dims; i++)
                size.p[i] = 0;
    }

    //! функция внутреннего использования, рассмотрите возможность использования вместо этого метода 'release'; освобождает данные матрицы
    void deallocate() {
        if(u) {
            UMatData* u_ = u;
            u = NULL;
            (u_->currAllocator ? u_->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u_);
        }
    }

    void copySize(const Mat &m) {
        setSize(*this, m.dims, 0, 0);
        for( int i = 0; i < dims; i++ )
        {
            size[i] = m.size[i];
            step[i] = m.step[i];
        }
    }
//    void reserve(size_t sz);
//    void reserveBuffer(size_t sz);
//    void resize(size_t sz);
//    void resize(size_t sz, const Scalar &s);
//    void push_back_(const void *elem);
//
//    template<typename _Tp>
//    void push_back(const _Tp &elem);
//    template<typename _Tp>
//    void push_back(const Mat_ <_Tp> &elem);
//    void push_back(const std::vector <_Tp> &elem);
//    void push_back(const Mat &m);
//    void pop_back(size_t nelems = 1);
//
//    void locateROI(Size &wholeSize, Point &ofs) const;
//    Mat &adjustROI(int dtop, int dbottom, int dleft, int dright);


//    Mat operator()(Range rowRange, Range colRange) const;
//    Mat operator()(const Rect &roi) const;
//    Mat operator()(const std::vector <Range> &ranges) const;

//    template<typename _Tp>
//    operator std::vector<_Tp>() const;
//    template<typename _Tp, int n>
//    operator Vec<_Tp, n>() const;
//    template<typename _Tp, int m, int n>
//    operator Matx<_Tp, m, n>() const;
//    template<typename _Tp, std::size_t _Nm>
//    operator std::array<_Tp, _Nm>() const;

    bool isContinuous() const;
    bool isSubmatrix() const;

    size_t elemSize() const;
    size_t elemSize1() const;

    int type() const;
    int depth() const;
    int channels() const;
    size_t step1(int i = 0) const;

    bool empty() const {
        return data == 0 || total() == 0 || dims == 0;
    }

    size_t total() const {
        if( dims <= 2 )
            return (size_t)rows * cols;
        size_t p = 1;
        for( int i = 0; i < dims; i++ )
            p *= size[i];
        return p;
    }

    size_t total(int startDim, int endDim = INT_MAX) const {
//        CV_Assert( 0 <= startDim && startDim <= endDim);
        size_t p = 1;
        int endDim_ = endDim <= dims ? endDim : dims;
        for( int i = startDim; i < endDim_; i++ )
            p *= size[i];
        return p;
    }

    uchar *ptr(int i0 = 0) {
//        CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
        return data + step.p[0] * i0;
    }

    const uchar *ptr(int i0 = 0) const {
        return data + step.p[0] * i0;
    }


//    uchar *ptr(int row, int col);
//    const uchar *ptr(int row, int col) const;
//
//    uchar *ptr(int i0, int i1, int i2);
//    const uchar *ptr(int i0, int i1, int i2) const;
//
//    uchar *ptr(const int *idx);
//    const uchar *ptr(const int *idx) const;
//
//    template<int n>
//    uchar *ptr(const Vec<int, n> &idx);
//    template<int n>
//    const uchar *ptr(const Vec<int, n> &idx) const;
//
//    template<typename _Tp>
//    _Tp *ptr(int i0 = 0);
//    template<typename _Tp>
//    const _Tp *ptr(int i0 = 0) const;
//
//    template<typename _Tp>
//    _Tp *ptr(int row, int col);
//    template<typename _Tp>
//    const _Tp *ptr(int row, int col) const;
//
//    template<typename _Tp>
//    _Tp *ptr(int i0, int i1, int i2);
//    template<typename _Tp>
//    const _Tp *ptr(int i0, int i1, int i2) const;
//
//    template<typename _Tp>
//    _Tp *ptr(const int *idx);
//    template<typename _Tp>
//    const _Tp *ptr(const int *idx) const;
//
//    template<typename _Tp, int n>
//    _Tp *ptr(const Vec<int, n> &idx);
//    template<typename _Tp, int n>
//    const _Tp *ptr(const Vec<int, n> &idx) const;
//
//    template<typename _Tp>
//    _Tp &at(int i0 = 0);
//
//    template<typename _Tp>
//    const _Tp &at(int i0 = 0) const;
//    template<typename _Tp>
//    _Tp &at(int row, int col);
//
//    template<typename _Tp>
//    const _Tp &at(int row, int col) const;
//    template<typename _Tp>
//    _Tp &at(int i0, int i1, int i2);
//    template<typename _Tp>
//    const _Tp &at(int i0, int i1, int i2) const;
//
//    /** @overload
//    @param idx Array of Mat::dims indices.
//    */
//    template<typename _Tp>
//    _Tp &at(const int *idx);
//
//    /** @overload
//    @param idx Array of Mat::dims indices.
//    */
//    template<typename _Tp>
//    const _Tp &at(const int *idx) const;
//
    template<typename _Tp, int n>
    _Tp &at(const Vec<int, n> &idx) {
        return *(_Tp*)ptr(idx.val);
    }

    template<typename _Tp, int n>
    const _Tp &at(const Vec<int, n> &idx) const {
        return *(const _Tp*)ptr(idx.val);
    }
//
//    /** @overload
//    special versions for 2D arrays (especially convenient for referencing image pixels)
//    @param pt Element position specified as Point(j,i) .
//    */
//    template<typename _Tp>
//    _Tp &at(Point pt);
//
//
//    template<typename _Tp>
//    const _Tp &at(Point pt) const;

//    template<typename _Tp>
//    MatIterator_ <_Tp> begin();
//
//    template<typename _Tp>
//    MatConstIterator_ <_Tp> begin() const;
//
//    /** @brief Same as begin() but for inverse traversal
//     */
//    template<typename _Tp>
//    std::reverse_iterator <MatIterator_<_Tp>> rbegin();
//
//    template<typename _Tp>
//    std::reverse_iterator <MatConstIterator_<_Tp>> rbegin() const;
//
//
//    template<typename _Tp>
//    MatIterator_ <_Tp> end();
//
//    template<typename _Tp>
//    MatConstIterator_ <_Tp> end() const;
//
//    /** @brief Same as end() but for inverse traversal
//     */
//    template<typename _Tp>
//    std::reverse_iterator <MatIterator_<_Tp>> rend();
//
//    template<typename _Tp>
//    std::reverse_iterator <MatConstIterator_<_Tp>> rend() const;


//    template<typename _Tp, typename Functor>
//    void forEach(const Functor &operation);
//
//    template<typename _Tp, typename Functor>
//    void forEach(const Functor &operation) const;
//
//    Mat(Mat &&m);
//
//    Mat &operator=(Mat &&m);
//
//    enum {
//        MAGIC_VAL = 0x42FF0000,
//        AUTO_STEP = 0,
//        CONTINUOUS_FLAG = CV_MAT_CONT_FLAG,
//        SUBMATRIX_FLAG = CV_SUBMAT_FLAG
//    };
//    enum {
//        MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7
//    };

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
    static MatAllocator *getStdAllocator() {
        CV_SINGLETON_LAZY_INIT(MatAllocator, new StdMatAllocator())
    }

    static MatAllocator *getDefaultAllocator() {
        return getDefaultAllocatorMatRef();
    }

//    static void setDefaultAllocator(MatAllocator *allocator);

    //! internal use method: updates the continuity flag
    void updateContinuityFlag() {
        flags = cv_updateContinuityFlag(flags, dims, size.p, step.p);
    }

    //! interaction with UMat
    UMatData *u;

    MatSize size;
    MatStep step;

protected:
//    template<typename _Tp, typename Functor>
//    void forEach_impl(const Functor &operation);
};

/////////////////////////////////////////////////////// MAt //////////////////////////////////////////////////

static MatAllocator*& getDefaultAllocatorMatRef()
{
    static MatAllocator* g_matAllocator = Mat::getStdAllocator();
    return g_matAllocator;
}

void fastFree(void* ptr) {
    if(ptr) {
        uchar* udata = ((uchar**)ptr)[-1];
//        CV_DbgAssert(udata < (uchar*)ptr &&
//                     ((uchar*)ptr - udata) <= (ptrdiff_t)(sizeof(void*)+CV_MALLOC_ALIGN));
        free(udata);
    }
}

template<typename _Tp>
static inline _Tp* alignPtr(_Tp* ptr, int n=(int)sizeof(_Tp)) {
//    CV_DbgAssert((n & (n - 1)) == 0); // n is a power of 2
    if ((n & (n - 1)) == 0) {
        throw ;
    }
    return (_Tp*)(((size_t)ptr + n-1) & -n);
}

void* fastMalloc(size_t size) {
    uchar* udata = (uchar*)malloc(size + sizeof(void*) + CV_MALLOC_ALIGN);
//    if(!udata)
//        return OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    return adata;
}

void setSize( Mat& m, int _dims, const int* _sz, const size_t* _steps, bool autoSteps)
{
//    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m.step.p != m.step.buf )
        {
            fastFree(m.step.p);
            m.step.p = m.step.buf;
            m.size.p = &m.rows;
        }
        if( _dims > 2 )
        {
            m.step.p = (size_t*)fastMalloc(_dims*sizeof(m.step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m.step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m.rows = m.cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CV_ELEM_SIZE(m.flags), esz1 = CV_ELEM_SIZE1(m.flags), total = esz;
    for( int i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
//        CV_Assert( s >= 0 );
        m.size.p[i] = s;

        if( _steps ) {
            if (i < _dims-1) {
                if (_steps[i] % esz1 != 0) {
                    throw ;
//                    CV_Error_(Error::BadStep, ("Step %zu for dimension %d must be a multiple of esz1 %zu", _steps[i], i, esz1));
                }

                m.step.p[i] = _steps[i];
            } else {
                m.step.p[i] = esz;
            }
        } else if( autoSteps ) {
            m.step.p[i] = total;
            uint64 total1 = (uint64)total*s;
            if( (uint64)total1 != (size_t)total1 ) throw ;
//                CV_Error( CV_StsOutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 ) {
        m.dims = 2;
        m.cols = 1;
        m.step[1] = esz;
    }
}

int cv_updateContinuityFlag(int flags, int dims, const int* size, const size_t* step) {
    int i, j;
    for( i = 0; i < dims; i++ )
    {
        if( size[i] > 1 )
            break;
    }

    uint64 t = (uint64)size[std::min(i, dims-1)]*CV_MAT_CN(flags);
    for( j = dims-1; j > i; j-- )
    {
        t *= size[j];
        if( step[j]*size[j] < step[j-1] )
            break;
    }

    if( j <= i && t == (uint64)(int)t )
        return flags | CV_MAT_CONT_FLAG;
    return flags & ~CV_MAT_CONT_FLAG;
}

void finalizeHdr(Mat& m)
{
    m.updateContinuityFlag();
    int d = m.dims;
    if( d > 2 )
        m.rows = m.cols = -1;
    if(m.u)
        m.datastart = m.data = m.u->data;
    if( m.data )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        if( m.size[0] > 0 )
        {
            m.dataend = m.ptr() + m.size[d-1]*m.step[d-1];
            for( int i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        else
            m.dataend = m.datalimit;
    }
    else
        m.dataend = m.datalimit = 0;
}

#endif //CVRANGEFINDER_MAT_H
