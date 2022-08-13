#ifndef CVRANGEFINDER_ENUM_STATUS_H
#define CVRANGEFINDER_ENUM_STATUS_H

#include "iostream"
#include <cstdlib>

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

enum InterpolationFlags {
    INTER_NEAREST        = 0,
    INTER_LINEAR         = 1,
    INTER_CUBIC          = 2,
    INTER_AREA           = 3,
    INTER_LANCZOS4       = 4,
    INTER_LINEAR_EXACT = 5,
    INTER_NEAREST_EXACT  = 6,
    INTER_MAX            = 7,
    WARP_FILL_OUTLIERS   = 8,
    WARP_INVERSE_MAP     = 16
};

enum ExifTagName {
    IMAGE_DESCRIPTION       = 0x010E,   ///< Image Description: ASCII string
    MAKE                    = 0x010F,   ///< Description of manufacturer: ASCII string
    MODEL                   = 0x0110,   ///< Description of camera model: ASCII string
    ORIENTATION             = 0x0112,   ///< Orientation of the image: unsigned short
    XRESOLUTION             = 0x011A,   ///< Resolution of the image across X axis: unsigned rational
    YRESOLUTION             = 0x011B,   ///< Resolution of the image across Y axis: unsigned rational
    RESOLUTION_UNIT         = 0x0128,   ///< Resolution units. '1' no-unit, '2' inch, '3' centimeter
    SOFTWARE                = 0x0131,   ///< Shows firmware(internal software of digicam) version number
    DATE_TIME               = 0x0132,   ///< Date/Time of image was last modified
    WHITE_POINT             = 0x013E,   ///< Chromaticity of white point of the image
    PRIMARY_CHROMATICIES    = 0x013F,   ///< Chromaticity of the primaries of the image
    Y_CB_CR_COEFFICIENTS    = 0x0211,   ///< constant to translate an image from YCbCr to RGB format
    Y_CB_CR_POSITIONING     = 0x0213,   ///< Chroma sample point of subsampling pixel array
    REFERENCE_BLACK_WHITE   = 0x0214,   ///< Reference value of black point/white point
    COPYRIGHT               = 0x8298,   ///< Copyright information
    EXIF_OFFSET             = 0x8769,   ///< Offset to Exif Sub IFD
    INVALID_TAG             = 0xFFFF    ///< Shows that the tag was not recognized
};

enum ImageOrientation
{
    IMAGE_ORIENTATION_TL = 1, ///< Horizontal (normal)
    IMAGE_ORIENTATION_TR = 2, ///< Mirrored horizontal
    IMAGE_ORIENTATION_BR = 3, ///< Rotate 180
    IMAGE_ORIENTATION_BL = 4, ///< Mirrored vertical
    IMAGE_ORIENTATION_LT = 5, ///< Mirrored horizontal & rotate 270 CW
    IMAGE_ORIENTATION_RT = 6, ///< Rotate 90 CW
    IMAGE_ORIENTATION_RB = 7, ///< Mirrored horizontal & rotate 90 CW
    IMAGE_ORIENTATION_LB = 8  ///< Rotate 270 CW
};

static inline const char * envRead(const char * name)
{
    return getenv(name);
}

template <typename T>
T parseOption(const std::string &);

template<>
inline bool parseOption(const std::string & value)
{
    if (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        return true;
    }
    if (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        return false;
    }
//    throw ParseError(value);
}

template<typename T>
inline T read(const std::string & k, const T & defaultValue)
{
    try {
        const char * res = envRead(k.c_str());
        if (res)
            return parseOption<T>(std::string(res));
    } catch (...) {
        std::cout << "erorr" << std::endl;
    }
    return defaultValue;
}

size_t getConfigurationParameterSizeT(const char* name, size_t defaultValue)
{
    return read<size_t>(name, defaultValue);
}

static const size_t CV_IO_MAX_IMAGE_PARAMS = getConfigurationParameterSizeT("OPENCV_IO_MAX_IMAGE_PARAMS", 50);
static const size_t CV_IO_MAX_IMAGE_WIDTH = getConfigurationParameterSizeT("OPENCV_IO_MAX_IMAGE_WIDTH", 1 << 20);
static const size_t CV_IO_MAX_IMAGE_HEIGHT = getConfigurationParameterSizeT("OPENCV_IO_MAX_IMAGE_HEIGHT", 1 << 20);
static const size_t CV_IO_MAX_IMAGE_PIXELS = getConfigurationParameterSizeT("OPENCV_IO_MAX_IMAGE_PIXELS", 1 << 30);


#endif //CVRANGEFINDER_ENUM_STATUS_H
