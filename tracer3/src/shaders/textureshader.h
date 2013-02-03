#ifndef __TEXTURESHADER_H__
#define __TEXTURESHADER_H__

#include "../tracer.h"

typedef unsigned int DWORD;
typedef char WORD;
typedef long LONG;

typedef struct BITMAPFILEHEADER { // bmfh 
    WORD    bfType; 
    DWORD   bfSize; 
    WORD    bfReserved1; 
    WORD    bfReserved2; 
    DWORD   bfOffBits; 
} BITMAPFILEHEADER; 

typedef struct BITMAPINFOHEADER{ // bmih 
    DWORD  biSize; 
    LONG   biWidth; 
    LONG   biHeight; 
    WORD   biPlanes; 
    WORD   biBitCount;
    DWORD  biCompression; 
    DWORD  biSizeImage; 
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
} BITMAPINFOHEADER; 

class TextureShader : public tracer::shader {
public:
	TextureShader(std::string filename = "", double _Ka = 0.0, double _Kd = 0.8, double _Kp = 0.6, double _phong_size = 20);
	virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
	virtual tracer::color transparency(const tracer::shade_context& context);
	tracer::color get(double s, double t);
	void loadRaw(std::string filename, int _width, int _height, int offset = 0);
	void loadBmp(std::string filename);
	virtual std::ostream& toStream(std::ostream& out) const;
	virtual std::istream& fromStream(std::istream& in);
	virtual tracer::shader* clone();
	std::string filename;
	double Ka, Kd, Kp, phong_size;
	int width, height;
	char *data;
};

#endif
