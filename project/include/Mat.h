#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include "SMatrix.h"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstddef>
#include <vector>
#include <string>
#include <stdio.h>
#include <setjmp.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>

extern "C" {  // jpeglib.h
#include <jconfig.h>
#include <jpeglib.h>
}

#define GRAY_CHANNEL 1
#define RGB_CHANNEL 3

/* Это сейчас пусть будет 0, тк эта библиотека не корректно работает. Вырезать ее тоже не стоит */
#define RASBERRY 0
#ifdef RASBERRY
#include <raspicam/raspicam.h>
#endif

typedef const float cfloat;
typedef unsigned char uchar;

typedef sstd::SMatrix<uchar> SM;

class Mat : public SM {
private:
    std::string filename;  /* Куда будет сохронятся картинка */
    int _channels;  /* По уолчанию картинка конвертируется в GRAY, но в будущем возможно использовать и RGB */

    /** \brief - метод переводит RGB изображение в серый
     * \R - красный
     * \G - зеленный
     * \B - синий
     * */
     uchar get_grey(const uchar &R, const uchar &G, const uchar &B) {
        return  uchar(R * float(0.299) + G * float(0.587) + B * float(0.114));
    }
public:
    /** \brief - Простой конструктор
     * */
    Mat() {}

    /** \brief - считывает из файла картинку и переводит ее в GRAY
     * \param filename - .jpg файл
     * */
    Mat(std::string filename) {
        struct jpeg_decompress_struct d1;
        struct jpeg_error_mgr m1;
        d1.err = jpeg_std_error(&m1);
        jpeg_create_decompress(&d1);
        FILE *f = fopen(filename.c_str(),"rb");
        jpeg_stdio_src(&d1, f);
        jpeg_read_header(&d1, TRUE);

        rows = d1.image_height;
        cols = d1.image_width;
        matrix = new uchar[rows * cols]{};
        this->filename = "test.jpg";
        this->_channels = GRAY_CHANNEL;


        jpeg_start_decompress(&d1);
        uchar *pBuf = new uchar[rows * cols * d1.num_components]{};
        for (size_t i = 0; d1.output_scanline < d1.output_height;) {
            // Получить экранную строку
            i += jpeg_read_scanlines(&d1, (JSAMPARRAY)&(pBuf), 1);
            for (size_t j = 0; j < cols; ++j) {
                matrix[j + (i - 1) * cols] = get_grey(pBuf[j * d1.num_components + 2],
                                                      pBuf[j * d1.num_components + 1],
                                                      pBuf[j * d1.num_components + 0]);
            }
        }

        jpeg_finish_decompress(&d1);
        jpeg_destroy_decompress(&d1);
        fclose(f);
        delete[] pBuf;
    }

#ifdef RASBERRY
    /** \brief - конструктор который делает снимок и переводит его в GRAY
     * НЕ РАБОТАЕТ, он не понятно что фотографирует
     * \param Camera - камера, raspicam
     * */
    Mat(raspicam::RaspiCam Camera) {
        Camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);
        Camera.open();
        usleep(3);
        Camera.grab();
        this->_channels = GRAY_CHANNEL;
        uchar *data = new uchar[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
        Camera.retrieve(data);
        rows = Camera.getHeight();
        cols = Camera.getWidth();
        matrix = new uchar[rows * cols]{};
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[j + i * cols] = get_grey(float(data[j + i * cols  + 2]),
                                                float(data[j + i * cols  + 1]),
                                                float(data[j + i * cols  + 0]));
            }
        }
        delete [] data;
    }
#endif

    /** \brief - деструктор
     * */
    ~Mat() {
        filename.clear();
    }

    /** \brief - сохраняет GRAY картинку.
     * */
    void write() {
        if (filename.empty()) {
            throw sstd::se::_without_file();
        }
        struct jpeg_compress_struct cinfo;  /* Шаг 1: выделите и инициализируйте объект сжатия JPEG */
        struct jpeg_error_mgr jerr;
        JSAMPROW row_pointer[1];
        cinfo.err = jpeg_std_error(&jerr);

        jpeg_create_compress(&cinfo);  /* Шаг 2: укажите место назначения данных  (eg, a file) */
        FILE *outfile = fopen(filename.c_str(),"wb");

        jpeg_stdio_dest(&cinfo, outfile); /* Шаг 3: установите параметры для сжатия */
        cinfo.image_width = cols;    /* Количество столбцов в изображении */
        cinfo.image_height = rows;   /* Количество строк в изображении */
        cinfo.input_components = this->_channels > 1 ? 3 : 1;     // Каналы, RGB 3 ; GRAY 1
        cinfo.in_color_space = this->_channels > 1 ? JCS_RGB : JCS_GRAYSCALE;
        JSAMPLE* image_buffer = new JSAMPLE[cols * rows * _channels]();  /* Указывает на большой массив данных R, G, B-порядка */
        for (size_t i = 0; i < cols * rows; i++) {
            image_buffer[i] = matrix[i];
        }
        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 100, true);

        jpeg_start_compress(&cinfo, true);  /* Шаг 4: Запустите компрессор */

        while (cinfo.next_scanline < cinfo.image_height) {  /* Шаг 5: пока (строки сканирования еще предстоит записать)*/
            row_pointer[0] = (JSAMPROW)&image_buffer[cinfo.next_scanline * cols * _channels];
            (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);  /* Шаг 6: Завершите сжатие */
        jpeg_destroy_compress(&cinfo);  /* Шаг 7: освободите объект сжатия JPEG */
        fclose(outfile);
    }

    /** \brief - сохраняет GRAY картинку по названию.
     * \param file - имя файла
     * */
    void write(std::string file) {
        filename = file;
        write();
    }

};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
