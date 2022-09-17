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

/*
 * Это сейчас пусть будет 0, тк эта библиотека не корректно работает. Вырезать ее тоже не стоит
 * */
#define RASBERRY 0
#ifdef RASBERRY
#include <raspicam/raspicam.h>
#endif

typedef const float cfloat;
typedef unsigned char uchar;

#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))

typedef sstd::SMatrix<int> SM;
typedef std::string String;
typedef unsigned int uint32_t;

template<typename _Tp, size_t fixed_size = 1024/sizeof(_Tp)+8>
class AutoBuffer {
public:
    typedef _Tp value_type;

    AutoBuffer() {
        ptr = buf;
        sz = fixed_size;
    }
    explicit AutoBuffer(size_t _size) {
        ptr = buf;
        sz = fixed_size;
        allocate(_size);
    }

    AutoBuffer(const AutoBuffer<_Tp, fixed_size>& abuf) {
        ptr = buf;
        sz = fixed_size;
        allocate(abuf.size());
        for( size_t i = 0; i < sz; i++ )
            ptr[i] = abuf.ptr[i];
    }
    AutoBuffer<_Tp, fixed_size>& operator = (const AutoBuffer<_Tp, fixed_size>& abuf) {
        if( this != &abuf )
        {
            deallocate();
            allocate(abuf.size());
            for( size_t i = 0; i < sz; i++ )
                ptr[i] = abuf.ptr[i];
        }
        return *this;
    }

    ~AutoBuffer() {
        deallocate();
    }

    void allocate(size_t _size) {
        if(_size <= sz)
        {
            sz = _size;
            return;
        }
        deallocate();
        sz = _size;
        if(_size > fixed_size)
        {
            ptr = new _Tp[_size];
        }
    }
    void deallocate() {
        if( ptr != buf )
        {
            delete[] ptr;
            ptr = buf;
            sz = fixed_size;
        }
    }
    void resize(size_t _size) {
        if(_size <= sz)
        {
            sz = _size;
            return;
        }
        size_t i, prevsize = sz, minsize = MIN(prevsize, _size);
        _Tp* prevptr = ptr;

        ptr = _size > fixed_size ? new _Tp[_size] : buf;
        sz = _size;

        if( ptr != prevptr )
            for( i = 0; i < minsize; i++ )
                ptr[i] = prevptr[i];
        for( i = prevsize; i < _size; i++ )
            ptr[i] = _Tp();

        if( prevptr != buf )
            delete[] prevptr;
    }
    size_t size() const {
        return sz;
    }
    inline _Tp* data() { return ptr; }
    inline const _Tp* data() const { return ptr; }

    operator _Tp* () { return ptr; }
    operator const _Tp* () const { return ptr; }

protected:
    _Tp* ptr;
    size_t sz;
    _Tp buf[(fixed_size > 0) ? fixed_size : 1];
};


class Mat : public SM {
private:
    String filename = 0;
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
    std::vector<uchar>* m_buf;
public:
    Mat(std::string filename) {
        struct jpeg_decompress_struct d1;
        struct jpeg_error_mgr m1;
        this->filename = filename;
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
        Camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);
        Camera.open();
        usleep(3);
        Camera.grab();
        uchar *data = new uchar[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
        Camera.retrieve (data);
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

//    bool write() {
//        if (filename.empty()) {
//            throw;
//        }
//        return write(filename);
//    }

//    bool write(String file) {
//        return write(this);
//    }

    /////////////////////// JpegEncoder ///////////////////
    uchar* getArray() {
        uchar *df = new uchar[cols*rows];
        for (size_t i = 0; i < cols*rows; i++) {
            df[i] = uchar(matrix[i]);
        }
        return df;
    }

    struct JpegErrorMgr
    {
        struct jpeg_error_mgr pub;
        jmp_buf setjmp_buffer;
    };

    struct JpegDestination {
        struct jpeg_destination_mgr pub;
        std::vector<uchar> *buf, *dst;
    };

//    METHODDEF(void) stub(j_decompress_ptr) {}


    METHODDEF(void) term_destination (j_compress_ptr cinfo) {
        JpegDestination* dest = (JpegDestination*)cinfo->dest;
        size_t sz = dest->dst->size(), bufsz = dest->buf->size() - dest->pub.free_in_buffer;
        if( bufsz > 0 ) {
            dest->dst->resize(sz + bufsz);
            memcpy( &(*dest->dst)[0] + sz, &(*dest->buf)[0], bufsz);
        }
    }

    METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo) {
        JpegDestination* dest = (JpegDestination*)cinfo->dest;
        size_t sz = dest->dst->size(), bufsz = dest->buf->size();
        dest->dst->resize(sz + bufsz);
        memcpy( &(*dest->dst)[0] + sz, &(*dest->buf)[0], bufsz);
        dest->pub.next_output_byte = &(*dest->buf)[0];
        dest->pub.free_in_buffer = bufsz;
        return true;
    }

    void icvCvt_BGR2RGB_8u_C3R(const uchar* bgr, int bgr_step, uchar* rgb, int rgb_step, int width, int height) {
        int i;
        for(; height--;){
            for( i = 0; i < width; i++, bgr += 3, rgb += 3 ) {
                uchar t0 = bgr[0], t1 = bgr[1], t2 = bgr[2];
                rgb[2] = t0; rgb[1] = t1; rgb[0] = t2;
            }
            bgr += bgr_step - width*3;
            rgb += rgb_step - width*3;
        }
    }

    static void jpeg_buffer_dest(j_compress_ptr cinfo, JpegDestination* destination)
    {
        cinfo->dest = &destination->pub;

//        destination->pub.init_destination = stub;
        destination->pub.empty_output_buffer = empty_output_buffer;
        destination->pub.term_destination = term_destination;
    }


    bool write(const Mat& img) {
//        m_last_error.clear();

        struct fileWrapper {
            FILE* f;

            fileWrapper() : f(0) {}
            ~fileWrapper() { if(f) fclose(f); }
        };
//        bool result = false;
        fileWrapper fw;
        int width = img.getCols(), height = img.getRows();

        std::vector<uchar> out_buf(1 << 12);
        AutoBuffer<uchar> _buffer;
        uchar* buffer;

        struct jpeg_compress_struct cinfo;
        JpegErrorMgr jerr;
        JpegDestination dest;

        cinfo.err = jpeg_std_error(&jerr.pub);
//        jerr.pub.error_exit = error_exit;
        jpeg_create_compress(&cinfo);

        if( !m_buf ) {
            fw.f = fopen( filename.c_str(), "wb" );
            if( !fw.f )
                throw;
            jpeg_stdio_dest( &cinfo, fw.f );
        } else {
            dest.dst = m_buf;
            dest.buf = &out_buf;

            jpeg_buffer_dest( &cinfo, &dest );

            dest.pub.next_output_byte = &out_buf[0];
            dest.pub.free_in_buffer = out_buf.size();
        }

        if( setjmp( jerr.setjmp_buffer ) == 0 ) {
            cinfo.image_width = width;
            cinfo.image_height = height;

            int _channels = 1; // img.channels(); // Каналы, RGB 3 ; GRAY 1
            int channels = _channels > 1 ? 3 : 1;
            cinfo.input_components = channels;
            cinfo.in_color_space = channels > 1 ? JCS_RGB : JCS_GRAYSCALE;

            int quality = 95;
            int progressive = 0;
            int optimize = 0;
            int rst_interval = 0;
            int luma_quality = -1;
            int chroma_quality = -1;
            uint32_t sampling_factor = 0; // same as 0x221111


            jpeg_set_defaults(&cinfo);
            cinfo.restart_interval = rst_interval;

            jpeg_set_quality(&cinfo, quality,
                             TRUE /* limit to baseline-JPEG values */ );
            if (progressive)
                jpeg_simple_progression(&cinfo);
//            if( optimize )
//                cinfo.optimize_coding = TRUE;

            if ((channels > 1) && (sampling_factor != 0)) {
                cinfo.comp_info[0].v_samp_factor = (sampling_factor >> 16) & 0xF;
                cinfo.comp_info[0].h_samp_factor = (sampling_factor >> 20) & 0xF;
                cinfo.comp_info[1].v_samp_factor = 1;
                cinfo.comp_info[1].h_samp_factor = 1;
            }

//#if JPEG_LIB_VERSION >= 70
//            if (luma_quality >= 0 && chroma_quality >= 0)
//        {
//            cinfo.q_scale_factor[0] = jpeg_quality_scaling(luma_quality);
//            cinfo.q_scale_factor[1] = jpeg_quality_scaling(chroma_quality);
//            if ( luma_quality != chroma_quality )
//            {
//                /* disable subsampling - ref. Libjpeg.txt */
//                cinfo.comp_info[0].v_samp_factor = 1;
//                cinfo.comp_info[0].h_samp_factor = 1;
//                cinfo.comp_info[1].v_samp_factor = 1;
//                cinfo.comp_info[1].h_samp_factor = 1;
//            }
//            jpeg_default_qtables( &cinfo, TRUE );
//        }
//#endif // #if JPEG_LIB_VERSION >= 70

            jpeg_start_compress(&cinfo, TRUE);

            if (channels > 1)
                _buffer.allocate(width * channels);
            buffer = _buffer.data();

            for (int y = 0; y < height; y++) {
                uchar *data = getArray(), *ptr = data;

                if (_channels == 3) {
                    icvCvt_BGR2RGB_8u_C3R(data, 0, buffer, 0, width, 1);
                    ptr = buffer;
                }
//                else if( _channels == 4 )
//                {
//                    icvCvt_BGRA2BGR_8u_C4C3R( data, 0, buffer, 0, Size(width,1), 2 );
//                    ptr = buffer;
//                }

                jpeg_write_scanlines(&cinfo, &ptr, 1);
            }

            jpeg_finish_compress(&cinfo);
//            result = true;
        }

        jpeg_destroy_compress( &cinfo );

        return true;
    }

};

#endif // CVRANGEFINDER_CV_TWO_ARRAY_H_
