#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "two_array.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdlib.h>     // подключаем qsort
#include <iostream>

using namespace TwoDimensionalArray;
using namespace cv;


/** пространство перевода картинки с RGB в серый
 * \img - картинка класс Mac
 * \row и  \col - координаты
 * */
namespace grey {

#define Rcof 0.2126
#define Gcof 0.7152
#define Bcof 0.0752

//https://habr.com/ru/post/353582/
//https://reddeveloper.ru/questions/kak-opencv-konvertirovat-izobrazheniye-v-ottenki-serogo-RyzkV
    static int get_grey(const Mat &img, size_t row, size_t col) {
        return Rcof * img.at<Vec3b>(row, col)[0] + Gcof * img.at<Vec3b>(row, col)[1] +
               Bcof * img.at<Vec3b>(row, col)[2];
    }
}

class CV_Array : public Array {
public:
    /** Конструктор перевода картинки в двумерный массив серого
     * */
    CV_Array(const Mat& img);

//    void sort();
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
            this->matrix[j + i * this->cols] = grey::get_grey(img, i, j);
        }
    }
};

////функция для сортировки по убыванию
//static int comp2 (const void * a, const void * b) {
//    return ( *(int *)b - *(int *)a );
//}
//
//void CV_Array::sort() {
//    std::qsort (this->matrix, this->rows * this->cols, sizeof(int), comp2);
//}

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
