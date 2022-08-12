#ifndef CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H
#define CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H

#include "Size.h"
#include "uchar_in_typedef.h"
#include "Enum_status.h"

#define CV_CN_MAX 512
#define CV_CN_SHIFT   3
#define CV_MAX_DIM            32
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)

#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)

#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE


#define CV_MAT_CN_MASK          ((CV_CN_MAX - 1) << CV_CN_SHIFT)
#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)
#define CV_MAT_TYPE_MASK        (CV_DEPTH_MAX*CV_CN_MAX - 1)
#define CV_MAT_TYPE(flags)      ((flags) & CV_MAT_TYPE_MASK)

static Size validateInputImageSize(const Size& size) {
    CV_Assert(size.width > 0);
    CV_Assert(static_cast<size_t>(size.width) <= CV_IO_MAX_IMAGE_WIDTH);
    CV_Assert(size.height > 0);
    CV_Assert(static_cast<size_t>(size.height) <= CV_IO_MAX_IMAGE_HEIGHT);
    uint64 pixels = (uint64)size.width * (uint64)size.height;
    CV_Assert(pixels <= CV_IO_MAX_IMAGE_PIXELS);
    return size;
}

#endif //CVRANGEFINDER_SIZE_VALIDATEINPUTIMAGESIZE_H
