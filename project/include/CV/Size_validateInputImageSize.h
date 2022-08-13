#ifndef CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H
#define CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H

#include "Size.h"
#include "uchar_in_typedef.h"
#include "Enum_status.h"
#include <atomic>

#define CV_CN_MAX 512
#define CV_CN_SHIFT   3
#define CV_MAX_DIM            32
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)
#define  CV_MALLOC_ALIGN    64

#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)

#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE


#define CV_MAT_CN_MASK          ((CV_CN_MAX - 1) << CV_CN_SHIFT)
#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)
#define CV_MAT_TYPE_MASK        (CV_DEPTH_MAX*CV_CN_MAX - 1)
#define CV_MAT_TYPE(flags)      ((flags) & CV_MAT_TYPE_MASK)

#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)

#define CV_ELEM_SIZE1(type) ((0x28442211 >> CV_MAT_DEPTH(type)*4) & 15)

#define CV_ELEM_SIZE(type) (CV_MAT_CN(type)*CV_ELEM_SIZE1(type))

#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_VALUE) \
    static TYPE* const instance = INITIALIZER; \
    return RET_VALUE;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)

#define CV_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)

#define CV_MAT_CONT_FLAG_SHIFT  14
#define CV_MAT_CONT_FLAG        (1 << CV_MAT_CONT_FLAG_SHIFT)
#define CV_AUTOSTEP  0x7fffffff
#define CV_8U   0
static Size validateInputImageSize(const Size& size) {
//    CV_Assert(size.width > 0);
//    CV_Assert(static_cast<size_t>(size.width) <= CV_IO_MAX_IMAGE_WIDTH);
//    CV_Assert(size.height > 0);
//    CV_Assert(static_cast<size_t>(size.height) <= CV_IO_MAX_IMAGE_HEIGHT);
    uint64 pixels = (uint64)size.width * (uint64)size.height;
//    CV_Assert(pixels <= CV_IO_MAX_IMAGE_PIXELS);
    return size;
}

#endif //CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H
