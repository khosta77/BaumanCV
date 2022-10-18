#ifndef CVRANGEFINDER_CV_TWO_ARRAY_H_
#define CVRANGEFINDER_CV_TWO_ARRAY_H_

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstddef>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <exception>

extern "C" {  // jpeglib.h
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <jconfig.h>
#include <jpeglib.h>
}

typedef unsigned char uchar;

class Mat {
private:
    size_t rows;      /* Строки */
    size_t cols;      /* Колонки */
    uchar *matrix;   /* Основной массив - он же матрица, т.к. двумерный */

    static inline uchar get_grey(const uchar &R, const uchar &G, const uchar &B) {
        return  uchar(float(R) * float(0.299) + float(G) * float(0.587) + float(B) * float(0.114));
    }
public:
    explicit Mat(const size_t &cols = 0, const size_t &rows = 0) : rows(rows), cols(cols) {
        matrix = new uchar[rows * cols]{};
    }

    Mat(const Mat& arr) {
        rows = arr.getRows();
        cols = arr.getCols();
        matrix = new uchar [rows * cols]{};
        for (size_t i = 0; i < (rows * cols); ++i) {
            matrix[i] = arr.matrix[i];
        }
    }

    Mat& operator=(const Mat& arr) {
        if (&arr != this) {
            delete[] this->matrix;
            rows = arr.getRows();
            cols = arr.getCols();
            matrix = new uchar[rows * cols]{};
            for (size_t i = 0; i < (rows * cols); ++i) {
                matrix[i] = arr.matrix[i];
            }
        }
        return *this;
    }

    explicit Mat(const std::string &filename) {
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

    ~Mat() {
        delete[] matrix;
    }

    void write(const std::string &filename) {

        struct jpeg_compress_struct cinfo;  /* Шаг 1: выделите и инициализируйте объект сжатия JPEG */
        struct jpeg_error_mgr jerr;
        JSAMPROW row_pointer[1];
        cinfo.err = jpeg_std_error(&jerr);

        jpeg_create_compress(&cinfo);  /* Шаг 2: укажите место назначения данных  (eg, a file) */
        FILE *outfile = fopen(filename.c_str(),"wb");

        jpeg_stdio_dest(&cinfo, outfile); /* Шаг 3: установите параметры для сжатия */
        cinfo.image_width = cols;    /* Количество столбцов в изображении */
        cinfo.image_height = rows;   /* Количество строк в изображении */
        cinfo.input_components = 1;     // Каналы, RGB 3 ; GRAY 1
        cinfo.in_color_space = J_COLOR_SPACE(1);
        JSAMPLE* image_buffer = new JSAMPLE[cols * rows]();  /* Указывает на большой массив данных R, G, B-порядка */
        for (size_t i = 0; i < cols * rows; ++i) {
            image_buffer[i] = matrix[i];
        }
        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 100, true);

        jpeg_start_compress(&cinfo, true);  /* Шаг 4: Запустите компрессор */

        while (cinfo.next_scanline < cinfo.image_height) {  /* Шаг 5: пока (строки сканирования еще предстоит записать)*/
            row_pointer[0] = (JSAMPROW)&image_buffer[cinfo.next_scanline * cols];
            (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);  /* Шаг 6: Завершите сжатие */
        jpeg_destroy_compress(&cinfo);  /* Шаг 7: освободите объект сжатия JPEG */
        fclose(outfile);
    }
//------------------------------------------------------------------------------------------------------------
    // Basic operations
    [[nodiscard]] inline size_t getRows() const noexcept {  return rows; }
    [[nodiscard]] inline size_t getCols() const noexcept {  return cols; }
    [[nodiscard]] inline size_t size() const noexcept {  return rows * cols; }

    uchar& operator()(const size_t &i, const size_t &j) const {
        return matrix[j + i * cols];
    }

    uchar& operator()(const size_t &i, const size_t &j) {
        return (uchar&)matrix[j + i * cols];
    }

    uchar& operator[](const size_t &k) noexcept { return matrix[k]; }
    uchar& operator[](const size_t &k) const noexcept { return matrix[k]; }

//------------------------------------------------------------------------------------------------------------
    void resize(const size_t &rows = 0, const size_t &cols = 0) {
        delete[] matrix;
        this->rows = rows;
        this->cols = cols;
        matrix = new uchar[rows * cols]{};
    }

    Mat submatrix(const size_t &ly, const size_t &ry, const size_t &lx, const size_t &rx) {
        Mat subM(rx - lx, ry - ly);
        for (size_t y = ly, iy = 0; y < ry; ++y, ++iy)
            for(size_t x = lx, ix = 0; x < rx; ++x, ++ix)
                subM.matrix[ix + iy * subM.cols] = this->matrix[x + y * cols];
        return subM;
    }
};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
