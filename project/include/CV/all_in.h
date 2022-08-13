#ifndef CVRANGEFINDER_ALL_IN_H
#define CVRANGEFINDER_ALL_IN_H

#include "Enum_status.h"
#include "Mat.h"
#include "BaseImageDecoder.h"
#include "Size_validateInputImageSize.h"
#include "UMat.h"

void resize(int src_type,
            const uchar * src_data, size_t src_step, int src_width, int src_height,
            uchar * dst_data, size_t dst_step, int dst_width, int dst_height,
            double inv_scale_x, double inv_scale_y, int interpolation)
{
    CV_INSTRUMENT_REGION();

    CV_Assert((dst_width > 0 && dst_height > 0) || (inv_scale_x > 0 && inv_scale_y > 0));
    if (inv_scale_x < DBL_EPSILON || inv_scale_y < DBL_EPSILON)
    {
        inv_scale_x = static_cast<double>(dst_width) / src_width;
        inv_scale_y = static_cast<double>(dst_height) / src_height;
    }

    CALL_HAL(resize, cv_hal_resize, src_type, src_data, src_step, src_width, src_height, dst_data, dst_step, dst_width, dst_height, inv_scale_x, inv_scale_y, interpolation);

    int  depth = CV_MAT_DEPTH(src_type), cn = CV_MAT_CN(src_type);
    Size dsize = Size(saturate_cast<int>(src_width*inv_scale_x),
                      saturate_cast<int>(src_height*inv_scale_y));
    CV_Assert( !dsize.empty() );

    CV_IPP_RUN_FAST(ipp_resize(src_data, src_step, src_width, src_height, dst_data, dst_step, dsize.width, dsize.height, inv_scale_x, inv_scale_y, depth, cn, interpolation))

    static ResizeFunc linear_tab[] =
            {
                    resizeGeneric_<
                    HResizeLinear<uchar, int, short,
                    INTER_RESIZE_COEF_SCALE,
                    HResizeLinearVec_8u32s>,
                    VResizeLinear<uchar, int, short,
                    FixedPtCast<int, uchar, INTER_RESIZE_COEF_BITS*2>,
                    VResizeLinearVec_32s8u> >,
                    0,
                    resizeGeneric_<
                    HResizeLinear<ushort, float, float, 1,
                    HResizeLinearVec_16u32f>,
                    VResizeLinear<ushort, float, float, Cast<float, ushort>,
                    VResizeLinearVec_32f16u> >,
                    resizeGeneric_<
                    HResizeLinear<short, float, float, 1,
                    HResizeLinearVec_16s32f>,
                    VResizeLinear<short, float, float, Cast<float, short>,
                    VResizeLinearVec_32f16s> >,
                    0,
                    resizeGeneric_<
                    HResizeLinear<float, float, float, 1,
                    HResizeLinearVec_32f>,
                    VResizeLinear<float, float, float, Cast<float, float>,
                    VResizeLinearVec_32f> >,
                    resizeGeneric_<
                    HResizeLinear<double, double, float, 1,
                    HResizeNoVec>,
                    VResizeLinear<double, double, float, Cast<double, double>,
                    VResizeNoVec> >,
                    0
            };

    static ResizeFunc cubic_tab[] =
            {
                    resizeGeneric_<
                    HResizeCubic<uchar, int, short>,
                    VResizeCubic<uchar, int, short,
                    FixedPtCast<int, uchar, INTER_RESIZE_COEF_BITS*2>,
                    VResizeCubicVec_32s8u> >,
                    0,
                    resizeGeneric_<
                    HResizeCubic<ushort, float, float>,
                    VResizeCubic<ushort, float, float, Cast<float, ushort>,
                    VResizeCubicVec_32f16u> >,
                    resizeGeneric_<
                    HResizeCubic<short, float, float>,
                    VResizeCubic<short, float, float, Cast<float, short>,
                    VResizeCubicVec_32f16s> >,
                    0,
                    resizeGeneric_<
                    HResizeCubic<float, float, float>,
                    VResizeCubic<float, float, float, Cast<float, float>,
                    VResizeCubicVec_32f> >,
                    resizeGeneric_<
                    HResizeCubic<double, double, float>,
                    VResizeCubic<double, double, float, Cast<double, double>,
                    VResizeNoVec> >,
                    0
            };

    static ResizeFunc lanczos4_tab[] =
            {
                    resizeGeneric_<HResizeLanczos4<uchar, int, short>,
                    VResizeLanczos4<uchar, int, short,
                    FixedPtCast<int, uchar, INTER_RESIZE_COEF_BITS*2>,
                    VResizeNoVec> >,
                    0,
                    resizeGeneric_<HResizeLanczos4<ushort, float, float>,
                    VResizeLanczos4<ushort, float, float, Cast<float, ushort>,
                    VResizeLanczos4Vec_32f16u> >,
                    resizeGeneric_<HResizeLanczos4<short, float, float>,
                    VResizeLanczos4<short, float, float, Cast<float, short>,
                    VResizeLanczos4Vec_32f16s> >,
                    0,
                    resizeGeneric_<HResizeLanczos4<float, float, float>,
                    VResizeLanczos4<float, float, float, Cast<float, float>,
                    VResizeLanczos4Vec_32f> >,
                    resizeGeneric_<HResizeLanczos4<double, double, float>,
                    VResizeLanczos4<double, double, float, Cast<double, double>,
                    VResizeNoVec> >,
                    0
            };

    static ResizeAreaFastFunc areafast_tab[] =
            {
                    resizeAreaFast_<uchar, int, ResizeAreaFastVec<uchar, ResizeAreaFastVec_SIMD_8u> >,
                    0,
                    resizeAreaFast_<ushort, float, ResizeAreaFastVec<ushort, ResizeAreaFastVec_SIMD_16u> >,
                    resizeAreaFast_<short, float, ResizeAreaFastVec<short, ResizeAreaFastVec_SIMD_16s> >,
                    0,
                    resizeAreaFast_<float, float, ResizeAreaFastVec_SIMD_32f>,
                    resizeAreaFast_<double, double, ResizeAreaFastNoVec<double, double> >,
                    0
            };

    static ResizeAreaFunc area_tab[] =
            {
                    resizeArea_<uchar, float>, 0, resizeArea_<ushort, float>,
                    resizeArea_<short, float>, 0, resizeArea_<float, float>,
                    resizeArea_<double, double>, 0
            };

    static be_resize_func linear_exact_tab[] =
            {
                    resize_bitExact<uchar, interpolationLinear<uchar> >,
                    resize_bitExact<schar, interpolationLinear<schar> >,
                    resize_bitExact<ushort, interpolationLinear<ushort> >,
                    resize_bitExact<short, interpolationLinear<short> >,
                    resize_bitExact<int, interpolationLinear<int> >,
                    0,
                    0,
                    0
            };

    double scale_x = 1./inv_scale_x, scale_y = 1./inv_scale_y;

    int iscale_x = saturate_cast<int>(scale_x);
    int iscale_y = saturate_cast<int>(scale_y);

    bool is_area_fast = std::abs(scale_x - iscale_x) < DBL_EPSILON &&
                        std::abs(scale_y - iscale_y) < DBL_EPSILON;

    Mat src(Size(src_width, src_height), src_type, const_cast<uchar*>(src_data), src_step);
    Mat dst(dsize, src_type, dst_data, dst_step);

    if (interpolation == INTER_LINEAR_EXACT)
    {
        // in case of inv_scale_x && inv_scale_y is equal to 0.5
        // INTER_AREA (fast) is equal to bit exact INTER_LINEAR
        if (is_area_fast && iscale_x == 2 && iscale_y == 2 && cn != 2)//Area resize implementation for 2-channel images isn't bit-exact
            interpolation = INTER_AREA;
        else
        {
            be_resize_func func = linear_exact_tab[depth];
            CV_Assert(func != 0);
            func(src_data, src_step, src_width, src_height,
                 dst_data, dst_step, dst_width, dst_height,
                 cn, inv_scale_x, inv_scale_y);
            return;
        }
    }

    if( interpolation == INTER_NEAREST )
    {
        resizeNN( src, dst, inv_scale_x, inv_scale_y );
        return;
    }

    if( interpolation == INTER_NEAREST_EXACT )
    {
        resizeNN_bitexact( src, dst, inv_scale_x, inv_scale_y );
        return;
    }

    int k, sx, sy, dx, dy;


    {
        // in case of scale_x && scale_y is equal to 2
        // INTER_AREA (fast) also is equal to INTER_LINEAR
        if( interpolation == INTER_LINEAR && is_area_fast && iscale_x == 2 && iscale_y == 2 )
            interpolation = INTER_AREA;

        // true "area" interpolation is only implemented for the case (scale_x >= 1 && scale_y >= 1).
        // In other cases it is emulated using some variant of bilinear interpolation
        if( interpolation == INTER_AREA && scale_x >= 1 && scale_y >= 1 )
        {
            if( is_area_fast )
            {
                int area = iscale_x*iscale_y;
                size_t srcstep = src_step / src.elemSize1();
                AutoBuffer<int> _ofs(area + dsize.width*cn);
                int* ofs = _ofs.data();
                int* xofs = ofs + area;
                ResizeAreaFastFunc func = areafast_tab[depth];
                CV_Assert( func != 0 );

                for( sy = 0, k = 0; sy < iscale_y; sy++ )
                    for( sx = 0; sx < iscale_x; sx++ )
                        ofs[k++] = (int)(sy*srcstep + sx*cn);

                for( dx = 0; dx < dsize.width; dx++ )
                {
                    int j = dx * cn;
                    sx = iscale_x * j;
                    for( k = 0; k < cn; k++ )
                        xofs[j + k] = sx + k;
                }

                func( src, dst, ofs, xofs, iscale_x, iscale_y );
                return;
            }

            ResizeAreaFunc func = area_tab[depth];
            CV_Assert( func != 0 && cn <= 4 );

            AutoBuffer<DecimateAlpha> _xytab((src_width + src_height)*2);
            DecimateAlpha* xtab = _xytab.data(), *ytab = xtab + src_width*2;

            int xtab_size = computeResizeAreaTab(src_width, dsize.width, cn, scale_x, xtab);
            int ytab_size = computeResizeAreaTab(src_height, dsize.height, 1, scale_y, ytab);

            AutoBuffer<int> _tabofs(dsize.height + 1);
            int* tabofs = _tabofs.data();
            for( k = 0, dy = 0; k < ytab_size; k++ )
            {
                if( k == 0 || ytab[k].di != ytab[k-1].di )
                {
                    CV_Assert( ytab[k].di == dy );
                    tabofs[dy++] = k;
                }
            }
            tabofs[dy] = ytab_size;

            func( src, dst, xtab, xtab_size, ytab, ytab_size, tabofs );
            return;
        }
    }

    int xmin = 0, xmax = dsize.width, width = dsize.width*cn;
    bool area_mode = interpolation == INTER_AREA;
    bool fixpt = depth == CV_8U;
    float fx, fy;
    ResizeFunc func=0;
    int ksize=0, ksize2;
    if( interpolation == INTER_CUBIC )
        ksize = 4, func = cubic_tab[depth];
    else if( interpolation == INTER_LANCZOS4 )
        ksize = 8, func = lanczos4_tab[depth];
    else if( interpolation == INTER_LINEAR || interpolation == INTER_AREA )
        ksize = 2, func = linear_tab[depth];
    else
        CV_Error( CV_StsBadArg, "Unknown interpolation method" );
    ksize2 = ksize/2;

    CV_Assert( func != 0 );

    AutoBuffer<uchar> _buffer((width + dsize.height)*(sizeof(int) + sizeof(float)*ksize));
    int* xofs = (int*)_buffer.data();
    int* yofs = xofs + width;
    float* alpha = (float*)(yofs + dsize.height);
    short* ialpha = (short*)alpha;
    float* beta = alpha + width*ksize;
    short* ibeta = ialpha + width*ksize;
    float cbuf[MAX_ESIZE] = {0};

    for( dx = 0; dx < dsize.width; dx++ )
    {
        if( !area_mode )
        {
            fx = (float)((dx+0.5)*scale_x - 0.5);
            sx = cvFloor(fx);
            fx -= sx;
        }
        else
        {
            sx = cvFloor(dx*scale_x);
            fx = (float)((dx+1) - (sx+1)*inv_scale_x);
            fx = fx <= 0 ? 0.f : fx - cvFloor(fx);
        }

        if( sx < ksize2-1 )
        {
            xmin = dx+1;
            if( sx < 0 && (interpolation != INTER_CUBIC && interpolation != INTER_LANCZOS4))
                fx = 0, sx = 0;
        }

        if( sx + ksize2 >= src_width )
        {
            xmax = std::min( xmax, dx );
            if( sx >= src_width-1 && (interpolation != INTER_CUBIC && interpolation != INTER_LANCZOS4))
                fx = 0, sx = src_width-1;
        }

        for( k = 0, sx *= cn; k < cn; k++ )
            xofs[dx*cn + k] = sx + k;

        if( interpolation == INTER_CUBIC )
            interpolateCubic( fx, cbuf );
        else if( interpolation == INTER_LANCZOS4 )
            interpolateLanczos4( fx, cbuf );
        else
        {
            cbuf[0] = 1.f - fx;
            cbuf[1] = fx;
        }
        if( fixpt )
        {
            for( k = 0; k < ksize; k++ )
                ialpha[dx*cn*ksize + k] = saturate_cast<short>(cbuf[k]*INTER_RESIZE_COEF_SCALE);
            for( ; k < cn*ksize; k++ )
                ialpha[dx*cn*ksize + k] = ialpha[dx*cn*ksize + k - ksize];
        }
        else
        {
            for( k = 0; k < ksize; k++ )
                alpha[dx*cn*ksize + k] = cbuf[k];
            for( ; k < cn*ksize; k++ )
                alpha[dx*cn*ksize + k] = alpha[dx*cn*ksize + k - ksize];
        }
    }

    for( dy = 0; dy < dsize.height; dy++ )
    {
        if( !area_mode )
        {
            fy = (float)((dy+0.5)*scale_y - 0.5);
            sy = cvFloor(fy);
            fy -= sy;
        }
        else
        {
            sy = cvFloor(dy*scale_y);
            fy = (float)((dy+1) - (sy+1)*inv_scale_y);
            fy = fy <= 0 ? 0.f : fy - cvFloor(fy);
        }

        yofs[dy] = sy;
        if( interpolation == INTER_CUBIC )
            interpolateCubic( fy, cbuf );
        else if( interpolation == INTER_LANCZOS4 )
            interpolateLanczos4( fy, cbuf );
        else
        {
            cbuf[0] = 1.f - fy;
            cbuf[1] = fy;
        }

        if( fixpt )
        {
            for( k = 0; k < ksize; k++ )
                ibeta[dy*ksize + k] = saturate_cast<short>(cbuf[k]*INTER_RESIZE_COEF_SCALE);
        }
        else
        {
            for( k = 0; k < ksize; k++ )
                beta[dy*ksize + k] = cbuf[k];
        }
    }

    func( src, dst, xofs, fixpt ? (void*)ialpha : (void*)alpha, yofs,
          fixpt ? (void*)ibeta : (void*)beta, xmin, xmax, ksize );
}

static void ExifTransform(int orientation, Mat& img)
{
    switch( orientation )
    {
        case    IMAGE_ORIENTATION_TL: //0th row == visual top, 0th column == visual left-hand side
            //do nothing, the image already has proper orientation
            break;
        case    IMAGE_ORIENTATION_TR: //0th row == visual top, 0th column == visual right-hand side
            flip(img, img, 1); //flip horizontally
            break;
        case    IMAGE_ORIENTATION_BR: //0th row == visual bottom, 0th column == visual right-hand side
            flip(img, img, -1);//flip both horizontally and vertically
            break;
        case    IMAGE_ORIENTATION_BL: //0th row == visual bottom, 0th column == visual left-hand side
            flip(img, img, 0); //flip vertically
            break;
        case    IMAGE_ORIENTATION_LT: //0th row == visual left-hand side, 0th column == visual top
            transpose(img, img);
            break;
        case    IMAGE_ORIENTATION_RT: //0th row == visual right-hand side, 0th column == visual top
            transpose(img, img);
            flip(img, img, 1); //flip horizontally
            break;
        case    IMAGE_ORIENTATION_RB: //0th row == visual right-hand side, 0th column == visual bottom
            transpose(img, img);
            flip(img, img, -1); //flip both horizontally and vertically
            break;
        case    IMAGE_ORIENTATION_LB: //0th row == visual left-hand side, 0th column == visual bottom
            transpose(img, img);
            flip(img, img, 0); //flip vertically
            break;
        default:
            //by default the image read has normal (JPEG_ORIENTATION_TL) orientation
            break;
    }
}

static void ApplyExifOrientation(ExifEntry_t orientationTag, Mat& img) {
    int orientation = IMAGE_ORIENTATION_TL;

    if (orientationTag.tag != INVALID_TAG) {
        orientation = orientationTag.field_u16; // ориентация короткая без знака, поэтому проверьте field_u16
        ExifTransform(orientation, img);
    }
}

void resize( InputArray _src, OutputArray _dst, Size dsize,
                 double inv_scale_x, double inv_scale_y, int interpolation )
{
//    CV_INSTRUMENT_REGION();

    Size ssize = _src.size();

//    CV_Assert( !ssize.empty() );
    if( dsize.empty() )
    {
//        CV_Assert(inv_scale_x > 0); CV_Assert(inv_scale_y > 0);
//        dsize = Size(saturate_cast<int>(ssize.width*inv_scale_x),
//                     saturate_cast<int>(ssize.height*inv_scale_y));
//        CV_Assert( !dsize.empty() );
    }
    else
    {
        inv_scale_x = (double)dsize.width/ssize.width;
        inv_scale_y = (double)dsize.height/ssize.height;
//        CV_Assert(inv_scale_x > 0); CV_Assert(inv_scale_y > 0);
    }

    if (interpolation == INTER_LINEAR_EXACT && (_src.depth() == CV_32F || _src.depth() == CV_64F))
        interpolation = INTER_LINEAR; // If depth isn't supported fallback to generic resize

//    CV_OCL_RUN(_src.dims() <= 2 && _dst.isUMat() && _src.cols() > 10 && _src.rows() > 10,
//               ocl_resize(_src, _dst, dsize, inv_scale_x, inv_scale_y, interpolation))

    // Fake reference to source. Resolves issue 13577 in case of src == dst.
    UMat srcUMat;
    if (_src.isUMat())
        srcUMat = _src.getUMat();

    Mat src = _src.getMat();
    _dst.create(dsize, src.type());
    Mat dst = _dst.getMat();

    if (dsize == ssize)
    {
        // Источник и пункт назначения имеют одинаковый размер. Используйте простое копирование.
        src.copyTo(dst);
        return;
    }

    resize(src.type(), src.data, src.step, src.cols, src.rows, dst.data, dst.step, dst.cols, dst.rows, inv_scale_x, inv_scale_y, interpolation);
}

#endif //CVRANGEFINDER_ALL_IN_H
