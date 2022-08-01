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

//        if (!img.data) {
//            throw std::exception();
//        }
//        this->rows = (size_t)img.rows;
//        this->cols = (size_t)img.cols;
//        this->matrix = new double[this->rows * this->cols];
//        for (size_t i = 0; i < this->rows; i++) {
//            for (size_t j = 0; j < this->cols; j++) {
//                this->matrix[j + i * this->cols] = get_grey(img, i, j);
//            }
//        }
    }
};

static bool imread_( const String& filename, int flags, Mat& mat ) {
    /// Search for the relevant decoder to handle the imagery
    ImageDecoder decoder;

    /// if no decoder was found, return nothing.
    if( !decoder ){
        return 0;
    }

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
    catch (const cv::Exception& e) {
        std::cerr << "imread_('" << filename << "'): can't read header: " << e.what() << std::endl << std::flush;
        return 0;
    }
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

    mat.create( size.height, size.width, type );

    // read the image data
    bool success = false;
    try
    {
        if (decoder->readData(mat))
            success = true;
    }
    catch (const cv::Exception& e)
    {
        std::cerr << "imread_('" << filename << "'): can't read data: " << e.what() << std::endl << std::flush;
    }
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

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
