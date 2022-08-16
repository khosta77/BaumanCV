#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "SMatrix.h"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstddef>
#include <string>
#include <stdio.h>
#include <setjmp.h>
#include <cstring>
#include <stdlib.h>

extern "C" {  // jpeglib.h
#include <jconfig.h>
#include <jpeglib.h>
}



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
        struct jpeg_decompress_struct d1;
        struct jpeg_error_mgr m1;

        d1.err = jpeg_std_error (&m1);
        jpeg_create_decompress(&d1);
        FILE *f = fopen(filename.c_str(),"rb");
        jpeg_stdio_src(&d1, f);
        jpeg_read_header(&d1, TRUE);

        rows = d1.image_height;
        cols = d1.image_width;
        matrix = new double[rows * cols]{};

        int num_s = d1.image_width * d1.image_height * d1.num_components;
        unsigned char *pBuf = (unsigned char*)malloc(num_s);
        memset(pBuf, 0, num_s);
        int i = 0, n_comp = d1.num_components;
        jpeg_start_decompress(&d1);
        while (d1.output_scanline < d1.output_height) {
            // Получить экранную строку
            i += jpeg_read_scanlines(&d1, (JSAMPARRAY)&(pBuf), 1);
            for (int j = 0; j < cols; j++) {
                matrix[j + (i - 1) * cols] = get_grey( pBuf[j * n_comp + 0], pBuf[j * n_comp + 1],
                                                       pBuf[j * n_comp + 2]);
            }
        }

        jpeg_finish_decompress(&d1);
        jpeg_destroy_decompress(&d1);
        fclose(f);
        free(pBuf);
    }

};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
