#include "utils.h"

namespace tracer {
  namespace util {
    void write16bit(tracer::color c, std::ostream& out) {
      unsigned short bits[3];
      
      bits[0] = (unsigned short)(65535*(1.0 - exp(-2*c.r)));
      bits[1] = (unsigned short)(65535*(1.0 - exp(-2*c.g)));
      bits[2] = (unsigned short)(65535*(1.0 - exp(-2*c.b)));
      
      for (int i=0;i<3;i++) {
	if ((bits[i] >> 8) == '\n' || (bits[i] & 0x00ff) == '\n') {
	  bits[i] += 1;
	}
      }

      out.write((char*)bits, 6);
    }

    void write8bit(tracer::color c, std::ostream& out) {
      unsigned char bits[3];

      if (c.r < 0 || c.g < 0 || c.b < 0) {
	std::cerr << "warning: color component below zero, " << c << std::endl;
	c = c.abs();
      }

      bits[0] = (unsigned char)(255*(1.0 - exp(-2*c.b)));
      bits[1] = (unsigned char)(255*(1.0 - exp(-2*c.g)));
      bits[2] = (unsigned char)(255*(1.0 - exp(-2*c.r)));
      //bits[0] = (unsigned char)(255*c.b);
      //bits[1] = (unsigned char)(255*c.g);
      //bits[2] = (unsigned char)(255*c.r);


      if (bits[0] == '\n') bits[0] += 1;
      if (bits[1] == '\n') bits[1] += 1;
      if (bits[2] == '\n') bits[2] += 1;
      
      out.write((char*)bits, 3);
    }

    void write8bit(unsigned char r, unsigned char g, unsigned char b, std::ostream& out) {
      if (r == '\n') r += 1;
      if (g == '\n') g += 1;
      if (b == '\n') b += 1;

      out.write((char*)&b, 1);
      out.write((char*)&g, 1);
      out.write((char*)&r, 1);
    }

    uint16 ushortToLittleEnd(uint16 i) {
      uint16 x = htons(i), ret;
      unsigned char *c1 = (unsigned char*)&x, *c2 = (unsigned char*)&ret;

      c2[0] = c1[1];
      c2[1] = c1[0];

      return ret;
    }

    uint32 ulongToLittleEnd(uint32 i) {
      uint32 x = htonl(i), ret;
      unsigned char *c1 = (unsigned char*)&x, *c2 = (unsigned char*)&ret;

      c2[0] = c1[3];
      c2[1] = c1[2];
      c2[2] = c1[1];
      c2[3] = c1[0];

      return ret;
    }

    uint16 ushortFromLittleEnd(uint16 i) {
      uint16 x = ntohs(i), ret;
      unsigned char *c1 = (unsigned char*)&x, *c2 = (unsigned char*)&ret;

      c2[0] = c1[1];
      c2[1] = c1[0];

      return ret;
    }

    uint32 ulongFromLittleEnd(uint32 i) {
      uint32 x = ntohl(i), ret;
      unsigned char *c1 = (unsigned char*)&x, *c2 = (unsigned char*)&ret;

      c2[0] = c1[3];
      c2[1] = c1[2];
      c2[2] = c1[1];
      c2[3] = c1[0];

      return ret;
    }
    
    void writeBmpHeader(unsigned int width, unsigned int height, unsigned short bitCount, std::ostream& out) {
      BITMAPFILEHEADER bmfh;
      BITMAPINFOHEADER bmih;

      bmfh.bfType          = ushortToLittleEnd(19778);
      bmfh.bfSize          = 0;//(bitCount/8)*width*height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
      bmfh.bfReserved1     = 0;
      bmfh.bfReserved2     = 0;
      bmfh.bfOffBits       = ulongToLittleEnd(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

      bmih.biSize          = ulongToLittleEnd(sizeof(BITMAPINFOHEADER));
      bmih.biWidth         = ulongToLittleEnd(width);
      bmih.biHeight        = ulongToLittleEnd(height);
      bmih.biPlanes        = ushortToLittleEnd(1);
      bmih.biBitCount      = ushortToLittleEnd(bitCount);
      bmih.biCompression   = 0;
      bmih.biSizeImage     = ulongToLittleEnd((bitCount/8)*width*height);
      bmih.biXPelsPerMeter = 0;
      bmih.biYPelsPerMeter = 0;
      bmih.biClrUsed       = 0;
      bmih.biClrImportant  = 0;

      out.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));
      out.write((char*)&bmih, sizeof(BITMAPINFOHEADER));
    }

    void readBmpHeader(std::string filename, BITMAPFILEHEADER& bmfh, BITMAPINFOHEADER& bmih) {
      FILE *file = fopen(filename.c_str(), "r");
      
      fread((void*)&bmfh, sizeof(BITMAPFILEHEADER), 1, file);		
      fread((void*)&bmih, sizeof(BITMAPINFOHEADER), 1, file);		

      fclose(file);

      bmfh.bfType      = ushortFromLittleEnd(bmfh.bfType);
      bmfh.bfSize      = ulongFromLittleEnd(bmfh.bfSize);
      bmfh.bfReserved1 = ushortFromLittleEnd(bmfh.bfReserved1);
      bmfh.bfReserved2 = ushortFromLittleEnd(bmfh.bfReserved2);
      bmfh.bfOffBits   = ulongFromLittleEnd(bmfh.bfOffBits);

      bmih.biSize          = ulongFromLittleEnd(bmih.biSize);
      bmih.biWidth         = ulongFromLittleEnd(bmih.biWidth);
      bmih.biHeight        = ulongFromLittleEnd(bmih.biHeight);
      bmih.biPlanes        = ushortFromLittleEnd(bmih.biPlanes);
      bmih.biBitCount      = ushortFromLittleEnd(bmih.biBitCount);
      bmih.biCompression   = ulongFromLittleEnd(bmih.biCompression);
      bmih.biSizeImage     = ulongFromLittleEnd(bmih.biSizeImage);
      bmih.biXPelsPerMeter = ulongFromLittleEnd(bmih.biXPelsPerMeter);
      bmih.biYPelsPerMeter = ulongFromLittleEnd(bmih.biYPelsPerMeter);
      bmih.biClrUsed       = ulongFromLittleEnd(bmih.biClrUsed);
      bmih.biClrImportant  = ulongFromLittleEnd(bmih.biClrImportant);
    }

    void printBmpHeader(std::string filename, std::ostream& out) {
      BITMAPFILEHEADER bmfh;
      BITMAPINFOHEADER bmih;
      
      readBmpHeader(filename, bmfh, bmih);
      
      out << "BITMAPFILEHEADER.bfType          = " << bmfh.bfType << std::endl;
      out << "BITMAPFILEHEADER.bfSize          = " << bmfh.bfSize << std::endl;
      out << "BITMAPFILEHEADER.bfReserved1     = " << bmfh.bfReserved1 << std::endl;
      out << "BITMAPFILEHEADER.bfReserved2     = " << bmfh.bfReserved2 << std::endl;
      out << "BITMAPFILEHEADER.bfOffBits       = " << bmfh.bfOffBits << std::endl;
      
      out << "BITMAPINFOHEADER.biSize          = " << bmih.biSize << std::endl;
      out << "BITMAPINFOHEADER.biWidth         = " << bmih.biWidth << std::endl;
      out << "BITMAPINFOHEADER.biHeight        = " << bmih.biHeight << std::endl;
      out << "BITMAPINFOHEADER.biPlanes        = " << bmih.biPlanes << std::endl;
      out << "BITMAPINFOHEADER.biBitCount      = " << bmih.biBitCount << std::endl;
      out << "BITMAPINFOHEADER.biCompression   = " << bmih.biCompression << std::endl;
      out << "BITMAPINFOHEADER.biSizeImage     = " << bmih.biSizeImage << std::endl;
      out << "BITMAPINFOHEADER.biXPelsPerMeter = " << bmih.biXPelsPerMeter << std::endl;
      out << "BITMAPINFOHEADER.biYPelsPerMeter = " << bmih.biYPelsPerMeter << std::endl;
      out << "BITMAPINFOHEADER.biClrUsed       = " << bmih.biClrUsed << std::endl;
      out << "BITMAPINFOHEADER.biClrImportant  = " << bmih.biClrImportant << std::endl;
    }
  } 
}






