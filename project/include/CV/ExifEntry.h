#ifndef CVRANGEFINDER_EXIFENTRY_H
#define CVRANGEFINDER_EXIFENTRY_H

#include "uchar_in_typedef.h"

struct ExifEntry_t
{
    ExifEntry_t();

    std::vector<u_rational_t> field_u_rational; ///< vector of rational fields
    std::string field_str;                      ///< any kind of textual information

    float  field_float;                         ///< Currently is not used
    double field_double;                        ///< Currently is not used

    uint32_t field_u32;                         ///< Unsigned 32-bit value
    int32_t  field_s32;                         ///< Signed 32-bit value

    uint16_t tag;                               ///< Tag number

    uint16_t field_u16;                         ///< Unsigned 16-bit value
    int16_t  field_s16;                         ///< Signed 16-bit value
    uint8_t  field_u8;                          ///< Unsigned 8-bit value
    int8_t   field_s8;                          ///< Signed 8-bit value
};

#endif //CVRANGEFINDER_EXIFENTRY_H
