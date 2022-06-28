#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "two_array.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace TwoDimensionalArray;
using namespace cv;


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
    static int get_grey(const Mat &img, size_t row, size_t col) {
        return Rcof * img.at<Vec3b>(row, col)[0] + Gcof * img.at<Vec3b>(row, col)[1] +
               Bcof * img.at<Vec3b>(row, col)[2];
    }
public:
    /** Конструктор перевода картинки в двумерный массив серого
     * */
    CV_Array(const Mat& img);

};

CV_Array::CV_Array(const Mat& img) {
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
};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
