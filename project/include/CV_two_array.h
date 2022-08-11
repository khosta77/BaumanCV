#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "two_array.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include "grfmt_base.hpp"

using namespace TwoDimensionalArray;
using namespace cv;

//! Imread flags
enum ImreadModes {
    IMREAD_UNCHANGED            = -1, //!< If set, return the loaded image as is (with alpha channel, otherwise it gets cropped). Ignore EXIF orientation.
    IMREAD_GRAYSCALE            = 0,  //!< If set, always convert image to the single channel grayscale image (codec internal conversion).
    IMREAD_COLOR                = 1,  //!< If set, always convert image to the 3 channel BGR color image.
    IMREAD_ANYDEPTH             = 2,  //!< If set, return 16-bit/32-bit image when the input has the corresponding depth, otherwise convert it to 8-bit.
    IMREAD_ANYCOLOR             = 4,  //!< If set, the image is read in any possible color format.
    IMREAD_LOAD_GDAL            = 8,  //!< If set, use the gdal driver for loading the image.
    IMREAD_REDUCED_GRAYSCALE_2  = 16, //!< If set, always convert image to the single channel grayscale image and the image size reduced 1/2.
    IMREAD_REDUCED_COLOR_2      = 17, //!< If set, always convert image to the 3 channel BGR color image and the image size reduced 1/2.
    IMREAD_REDUCED_GRAYSCALE_4  = 32, //!< If set, always convert image to the single channel grayscale image and the image size reduced 1/4.
    IMREAD_REDUCED_COLOR_4      = 33, //!< If set, always convert image to the 3 channel BGR color image and the image size reduced 1/4.
    IMREAD_REDUCED_GRAYSCALE_8  = 64, //!< If set, always convert image to the single channel grayscale image and the image size reduced 1/8.
    IMREAD_REDUCED_COLOR_8      = 65, //!< If set, always convert image to the 3 channel BGR color image and the image size reduced 1/8.
    IMREAD_IGNORE_ORIENTATION   = 128 //!< If set, do not rotate the image according to EXIF's orientation flag.
};


//! interpolation algorithm
enum InterpolationFlags{
    /** nearest neighbor interpolation */
    INTER_NEAREST        = 0,
    /** bilinear interpolation */
    INTER_LINEAR         = 1,
    /** bicubic interpolation */
    INTER_CUBIC          = 2,
    /** resampling using pixel area relation. It may be a preferred method for image decimation, as
    it gives moire'-free results. But when the image is zoomed, it is similar to the INTER_NEAREST
    method. */
    INTER_AREA           = 3,
    /** Lanczos interpolation over 8x8 neighborhood */
    INTER_LANCZOS4       = 4,
    /** Bit exact bilinear interpolation */
    INTER_LINEAR_EXACT = 5,
    /** Bit exact nearest neighbor interpolation. This will produce same results as
    the nearest neighbor method in PIL, scikit-image or Matlab. */
    INTER_NEAREST_EXACT  = 6,
    /** mask for interpolation codes */
    INTER_MAX            = 7,
    /** flag, fills all of the destination image pixels. If some of them correspond to outliers in the
    source image, they are set to zero */
    WARP_FILL_OUTLIERS   = 8,
    /** flag, inverse transformation

    For example, #linearPolar or #logPolar transforms:
    - flag is __not__ set: \f$dst( \rho , \phi ) = src(x,y)\f$
    - flag is set: \f$dst(x,y) = src( \rho , \phi )\f$
    */
    WARP_INVERSE_MAP     = 16
};

/**
 * @brief Base Exif tags used by IFD0 (main image)
 */
enum ExifTagName
{
    IMAGE_DESCRIPTION       = 0x010E,   ///< Image Description: ASCII string
    MAKE                    = 0x010F,   ///< Description of manufacturer: ASCII string
    MODEL                   = 0x0110,   ///< Description of camera model: ASCII string
    ORIENTATION             = 0x0112,   ///< Orientation of the image: unsigned short
    XRESOLUTION             = 0x011A,   ///< Resolution of the image across X axis: unsigned rational
    YRESOLUTION             = 0x011B,   ///< Resolution of the image across Y axis: unsigned rational
    RESOLUTION_UNIT         = 0x0128,   ///< Resolution units. '1' no-unit, '2' inch, '3' centimeter
    SOFTWARE                = 0x0131,   ///< Shows firmware(internal software of digicam) version number
    DATE_TIME               = 0x0132,   ///< Date/Time of image was last modified
    WHITE_POINT             = 0x013E,   ///< Chromaticity of white point of the image
    PRIMARY_CHROMATICIES    = 0x013F,   ///< Chromaticity of the primaries of the image
    Y_CB_CR_COEFFICIENTS    = 0x0211,   ///< constant to translate an image from YCbCr to RGB format
    Y_CB_CR_POSITIONING     = 0x0213,   ///< Chroma sample point of subsampling pixel array
    REFERENCE_BLACK_WHITE   = 0x0214,   ///< Reference value of black point/white point
    COPYRIGHT               = 0x8298,   ///< Copyright information
    EXIF_OFFSET             = 0x8769,   ///< Offset to Exif Sub IFD
    INVALID_TAG             = 0xFFFF    ///< Shows that the tag was not recognized
};

typedef std::string String;

static bool imread_( const String& filename, int flags, Mat& mat ) {
    /// Search for the relevant decoder to handle the imagery
    ImageDecoder decoder;

    /// if no decoder was found, return nothing.
//    if( !decoder ){
//        return 0;
//    }

    int scale_denom = 1;
    if( flags > IMREAD_LOAD_GDAL )
    {
        if( flags & IMREAD_REDUCED_GRAYSCALE_2 )
            scale_denom = 2;
        else if( flags & IMREAD_REDUCED_GRAYSCALE_4 )
            scale_denom = 4;
        else if( flags & IMREAD_REDUCED_GRAYSCALE_8 )
            scale_denom = 8;
    }

    /// set the scale_denom in the driver
    decoder->setScale( scale_denom );

    /// set the filename in the driver
    decoder->setSource( filename );

    try {
        // read the header to make sure it succeeds
        if( !decoder->readHeader() )
            return 0;
    }
//    catch (const cv::Exception& e) {
//        std::cerr << "imread_('" << filename << "'): can't read header: " << e.what() << std::endl << std::flush;
//        return 0;
//    }
    catch (...)
    {
        std::cerr << "imread_('" << filename << "'): can't read header: unknown exception" << std::endl << std::flush;
        return 0;
    }


    // established the required input image size
    Size size = validateInputImageSize(Size(decoder->width(), decoder->height()));

    // grab the decoded type
    int type = decoder->type();
    if( (flags & IMREAD_LOAD_GDAL) != IMREAD_LOAD_GDAL && flags != IMREAD_UNCHANGED )
    {
        if( (flags & IMREAD_ANYDEPTH) == 0 )
            type = CV_MAKETYPE(CV_8U, CV_MAT_CN(type));

        if( (flags & IMREAD_COLOR) != 0 ||
            ((flags & IMREAD_ANYCOLOR) != 0 && CV_MAT_CN(type) > 1) )
            type = CV_MAKETYPE(CV_MAT_DEPTH(type), 3);
        else
            type = CV_MAKETYPE(CV_MAT_DEPTH(type), 1);
    }

//    Mat mat;
    mat.create( size.height, size.width, type );

    // read the image data
    bool success = false;
    try
    {
        if (decoder->readData(mat))
            success = true;
    }
//    catch (const cv::Exception& e)
//    {
//        std::cerr << "imread_('" << filename << "'): can't read data: " << e.what() << std::endl << std::flush;
//    }
    catch (...)
    {
        std::cerr << "imread_('" << filename << "'): can't read data: unknown exception" << std::endl << std::flush;
    }
    if (!success)
    {
        mat.release();
        return false;
    }

    if( decoder->setScale( scale_denom ) > 1 ) // if decoder is JpegDecoder then decoder->setScale always returns 1
    {
        resize( mat, mat, Size( size.width / scale_denom, size.height / scale_denom ), 0, 0, INTER_LINEAR_EXACT);
    }

    /// optionally rotate the data if EXIF orientation flag says so
    if (!mat.empty() && (flags & IMREAD_IGNORE_ORIENTATION) == 0 && flags != IMREAD_UNCHANGED )
    {
        ApplyExifOrientation(decoder->getExifTag(ORIENTATION), mat);
    }

    return true;
}

class CV_Array : public Array {
private:
    /** Значения коэффицентов взяты с официальной документации
     * https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html
     * */
    const double Rcof = 0.299;
    const double Gcof = 0.587;
    const double Bcof = 0.114;

    /** пространство перевода картинки с RGB в серый
     * \img - картинка класс Mac
     * \row и  \col - координаты
     * */
    int get_grey(const Mat &img, size_t row, size_t col) {
        return Rcof * img.at<Vec3b>(row, col)[0] + Gcof * img.at<Vec3b>(row, col)[1] +
               Bcof * img.at<Vec3b>(row, col)[2];
    }
public:
    /** Конструктор перевода картинки в двумерный массив серого
     * */
    CV_Array(const std::string& filename, int flags) {
        Mat img;
        String str = filename;
        imread_(str, flags, img);
        if (!img.data) {
            throw std::exception();
        }
        this->rows = (size_t)img.rows;
        this->cols = (size_t)img.cols;
        this->matrix = new double[this->rows * this->cols];
        for (size_t i = 0; i < this->rows; i++) {
            for (size_t j = 0; j < this->cols; j++) {
                this->matrix[j + i * this->cols] = get_grey(img, i, j);
            }
        }
    }
};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
