#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "SMatrix.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstddef>
#include <string>
////////////////////////
#include <iostream>
#include <stdio.h>
#include <setjmp.h>

extern "C" {
//#include "jpeglib.h"
#include <jconfig.h>
//#include <jinclude.h>
#include <jpeglib.h>

}

#include <cstring>
#include <stdlib.h>
///

typedef sstd::SMatrix<double> SM;

class Mat : public SM {
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
    int get_grey(const double &R, const double &G, const double &B) {
        return  Rcof * R + Gcof * G + Bcof * B;
    }
public:
    Mat(std::string filename) {
//        std::cout << "1" << std::endl;
        struct jpeg_decompress_struct d1;
        struct jpeg_error_mgr m1;

        d1.err = jpeg_std_error (&m1);
        jpeg_create_decompress(&d1);
        FILE *f = fopen(filename.c_str(),"rb");
        jpeg_stdio_src(&d1, f);
        jpeg_read_header(&d1, TRUE);
//        std::cout << "2" << std::endl;
        rows = d1.image_height;
        cols = d1.image_width;
        matrix = new double[rows * cols]{};
//        for (size_t i = 0; i < (rows * cols); i++) {
//            matrix[i] = arr.matrix[i];
//        }
//        std::cout << "3) " << cols << " " << rows << std::endl;
//        Mat df(d/1.image_width, d1.image_height);


//        int w,h;
//        int n;
//        w = d1.image_width;
//        h = d1.image_height;
//        n = d1.num_components;
//        printf ("Passed %d x %d x %d.\n", w, h, n);

//        int n_s = w * n; // RGB, num. of the samples
//        unsigned char *pBuf;
//        unsigned char aR,aG,aB;
//        unsigned long sR = 0;
//        unsigned long sG = 0;
//        unsigned long sB = 0;

        int num_s = d1.image_width * d1.image_height * d1.num_components;
        unsigned char *pBuf = (unsigned char*)malloc(num_s);
        memset(pBuf, 0, num_s);
//        std::cout << "4" << std::endl;
        int i = 0, n_comp = d1.num_components;
        jpeg_start_decompress(&d1);
//        std::cout << "4.5" << std::endl;
        while (d1.output_scanline < d1.output_height) {
            // Получить экранную строку
            i += jpeg_read_scanlines(&d1, (JSAMPARRAY)&(pBuf), 1);
//        std::cout << i << " ";
//            std::cout << d1.output_scanline << " " << d1.output_height << std::endl;
            for (int j = 0; j < cols; j++) {
//                sR = pBuf[j*n + 0];
//                sG = pBuf[j*n + 1];
//                sB = pBuf[j*n + 2];
//        std::cout << (i - 1) << " " << j << std::endl;
//if ((j + (i - 1) * cols) >= (rows * cols - 1)) {
//    std::cout << (i - 1) << " " << j << std::endl;
//}
                matrix[j + (i - 1) * cols] = get_grey( pBuf[j * n_comp + 0], pBuf[j * n_comp + 1],
                                                       pBuf[j * n_comp + 2]);
            }
        }
//        std::cout << "5" << std::endl;
//    cout << endl;
//    aR = sR/(w*h);
//    aG = sG/(w*h);
//    aB = sB/(w*h);
//    printf("%02X %02X %02X\n",aR,aG,aB);
        jpeg_finish_decompress(&d1);
        jpeg_destroy_decompress(&d1);
        fclose(f);
        free(pBuf);

//        this = df;
    }

};
//#include "two_array.h"

//using namespace TwoDimensionalArray;
//
//class CV_Array : public Array {
//private:
//    /** Значения коэффицентов взяты с официальной документации
//     * https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html
//     * */
//    const double Rcof = 0.299;
//    const double Gcof = 0.587;
//    const double Bcof = 0.114;
//
//    /** пространство перевода картинки с RGB в серый
//     * \img - картинка класс Mac
//     * \row и  \col - координаты
//     * */
////    int get_grey(const Mat &img, size_t row, size_t col) {
////        return 1 ; // Rcof * img.at<Vec3b>(row, col)[0] + Gcof * img.at<Vec3b>(row, col)[1] + \
////               Bcof * img.at<Vec3b>(row, col)[2];
////    }
//public:
//    /** Конструктор перевода картинки в двумерный массив серого
//     * */
//    CV_Array() {
////        Mat img;
////        String str = filename;
////        imread_(str, flags, img);
////        if (!img.data) {
////            throw std::exception();
////        }
////        this->rows = (size_t)img.rows;
////        this->cols = (size_t)img.cols;
////        this->matrix = new double[this->rows * this->cols];
////        for (size_t i = 0; i < this->rows; i++) {
////            for (size_t j = 0; j < this->cols; j++) {
////                this->matrix[j + i * this->cols] = get_grey(img, i, j);
////            }
////        }
//    }
//};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
