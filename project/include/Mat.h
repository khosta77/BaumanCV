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

#include <raspicam/raspicam.h>
#define RASBERRY 1


typedef const float cfloat;
typedef unsigned char uchar;

typedef sstd::SMatrix<int> SM;

class Mat : public SM {
private:
    /** Значения коэффицентов взяты с официальной документации
     * https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html
     * */
    cfloat Rcof = 0.299;
    cfloat Gcof = 0.587;
    cfloat Bcof = 0.114;

    /** \brief - метод переводит RGB изображение в серый
     * \R - красный
     * \G - зеленный
     * \B - синий
     * */
    int get_grey(cfloat &R, cfloat &G, cfloat &B) {
        return  int(Rcof * R + Gcof * G + Bcof * B);
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
        matrix = new int[rows * cols]{};

        uchar *pBuf = new uchar[rows * cols * d1.num_components]{};

        jpeg_start_decompress(&d1);
        for (size_t i = 0; d1.output_scanline < d1.output_height;) {
            // Получить экранную строку
            i += jpeg_read_scanlines(&d1, (JSAMPARRAY)&(pBuf), 1);
            for (size_t j = 0; j < cols; j++) {
                matrix[j + (i - 1) * cols] = get_grey( float(pBuf[j * d1.num_components + 2]),
                                                       float(pBuf[j * d1.num_components + 1]),
                                                       float(pBuf[j * d1.num_components + 0]));
            }
        }

        jpeg_finish_decompress(&d1);
        jpeg_destroy_decompress(&d1);
        fclose(f);
        delete[] pBuf;
    }
#ifdef RASBERRY
    Mat(raspicam::RaspiCam Camera) {
        Camera.grab();
        Camera.grab();
        uchar *data = new uchar[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
        Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );
        rows = Camera.getHeight();
        cols = Camera.getWidth();
        matrix = new int[rows * cols]{};
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[j + i * cols] = get_grey(float(data[j + i * cols + 2]),
                                                float(data[j + i * cols + 1]),
                                                float(data[j + i * cols + 0]));
            }
        }
        delete [] data;
    }
#endif
};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
