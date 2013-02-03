#ifndef __UTILS_H__
#define __UTILS_H__

#include <netinet/in.h>
#include "color.h"

#pragma pack (2)

typedef unsigned short uint16;
typedef unsigned int uint32;

namespace tracer {
  namespace util {

    typedef struct {
      uint16 bfType; 
      uint32 bfSize; 
      uint16 bfReserved1; 
      uint16 bfReserved2; 
      uint32 bfOffBits; 
    } BITMAPFILEHEADER; 
    
    typedef struct { 
      uint32 biSize;
      uint32 biWidth; 
      uint32 biHeight; 
      uint16 biPlanes; 
      uint16 biBitCount; 
      uint32 biCompression; 
      uint32 biSizeImage; 
      uint32 biXPelsPerMeter; 
      uint32 biYPelsPerMeter; 
      uint32 biClrUsed; 
      uint32 biClrImportant; 
    } BITMAPINFOHEADER; 

    typedef struct {
      unsigned char r;
      unsigned char g;
      unsigned char b;
    } rgb;
    
    void write16bit(tracer::color c, std::ostream& out);

    void write8bit(tracer::color c, std::ostream& out);
    
    void write8bit(unsigned char r, unsigned char g, unsigned char b, std::ostream& out);

    uint16 ushortToLittleEnd(uint16 i);
    
    uint32 ulongToLittleEnd(uint32 i);
    
    uint16 ushortFromLittleEnd(uint16 i);
    
    uint32 ulongFromLittleEnd(uint32 i);
    
    void writeBmpHeader(unsigned int width, unsigned int height, unsigned short bitCount, std::ostream& out);
    
    void readBmpHeader(std::string filename, BITMAPFILEHEADER& bmfh, BITMAPINFOHEADER& bmih);
    
    void printBmpHeader(std::string filename, std::ostream& out);
  } 
}

#endif




