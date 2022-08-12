#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "two_array.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstddef>
#include "./CV/all_in.h"


using namespace TwoDimensionalArray;


static bool imread_( const String& filename, int flags, Mat& mat ) {
    ImageDecoder decoder;

    int scale_denom = 1;
    if( flags > IMREAD_LOAD_GDAL ) {
        if( flags & IMREAD_REDUCED_GRAYSCALE_2 ) {
            scale_denom = 2;
        } else if( flags & IMREAD_REDUCED_GRAYSCALE_4 ) {
            scale_denom = 4;
        } else if( flags & IMREAD_REDUCED_GRAYSCALE_8 ) {
            scale_denom = 8;
        }
    }

    decoder->setScale( scale_denom );
    decoder->setSource( filename );

    try {
        if( !decoder->readHeader() ) {
            return 0;
        }
    } catch (...) {
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
    try {
        if (decoder->readData(mat)) {
            success = true;
        }
    } catch (...) {
        std::cerr << "imread_('" << filename << "'): can't read data: unknown exception" << std::endl << std::flush;
    }

    if (!success) {
        mat.release();
        return false;
    }

    if( decoder->setScale( scale_denom ) > 1 ) {
        resize( mat, mat, Size( size.width / scale_denom, size.height / scale_denom ), 0, 0, INTER_LINEAR_EXACT);
    }

    if (!mat.empty() && (flags & IMREAD_IGNORE_ORIENTATION) == 0 && flags != IMREAD_UNCHANGED ) {
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
