#ifndef CVRANGEFINDER_BASEIMAGEDECODER_H
#define CVRANGEFINDER_BASEIMAGEDECODER_H

#include "Mat.h"
#include "Enum_status.h"
#include "Ptr.h"
#include "Size.h"
#include "ExifEntry.h"

class BaseImageDecoder;
//class BaseImageEncoder;
//typedef Ptr<BaseImageEncoder> ImageEncoder;
///////////////////////////////// base class for decoders ////////////////////////
class BaseImageDecoder
{
public:
    BaseImageDecoder(); // вызывается
    virtual ~BaseImageDecoder() {}

    int width() const { return m_width; }  // вызывается
    int height() const { return m_height; }  // вызывается
    virtual int type() const { return m_type; } // вызывается

    ExifEntry_t getExifTag(const ExifTagName tag) const;
    virtual bool setSource( const String& filename );    // вызывается
    virtual bool setSource( const Mat& buf );
    virtual int setScale( const int& scale_denom ); // вызывается
    virtual bool readHeader() = 0;  // вызывается
    virtual bool readData( Mat& img ) = 0;

    /// Called after readData to advance to the next page, if any.
    virtual bool nextPage() { return false; }

    virtual size_t signatureLength() const;
    virtual bool checkSignature( const String& signature ) const;
    virtual ImageDecoder newDecoder() const;

protected:
    int  m_width;  // width  of the image ( filled by readHeader )
    int  m_height; // height of the image ( filled by readHeader )
    int  m_type;
    int  m_scale_denom;
    String m_filename;
    String m_signature;
    Mat m_buf;
    bool m_buf_supported;
//    ExifReader m_exif;
};

BaseImageDecoder::BaseImageDecoder()
{
    m_width = m_height = 0;
    m_type = -1;
    m_buf_supported = false;
    m_scale_denom = 1;
}


ExifEntry_t BaseImageDecoder::getExifTag(const ExifTagName tag) const
{
    return m_exif.getTag(tag);
}
bool BaseImageDecoder::setSource( const String& filename )
{
    m_filename = filename;
    m_buf.release();
    return true;
}

bool BaseImageDecoder::setSource( const Mat& buf )
{
    if( !m_buf_supported )
        return false;
    m_filename = String();
    m_buf = buf;
    return true;
}

size_t BaseImageDecoder::signatureLength() const
{
    return m_signature.size();
}

bool BaseImageDecoder::checkSignature( const String& signature ) const
{
    size_t len = signatureLength();
    return signature.size() >= len && memcmp( signature.c_str(), m_signature.c_str(), len ) == 0;
}

int BaseImageDecoder::setScale( const int& scale_denom )
{
    int temp = m_scale_denom;
    m_scale_denom = scale_denom;
    return temp;
}

typedef Ptr<BaseImageDecoder> ImageDecoder;

ImageDecoder BaseImageDecoder::newDecoder() const
{
    return ImageDecoder();
}

#endif //CVRANGEFINDER_BASEIMAGEDECODER_H
