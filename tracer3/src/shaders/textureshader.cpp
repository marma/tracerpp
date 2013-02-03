#include "textureshader.h"

TextureShader::TextureShader(std::string _filename, double _Ka, double _Kd, double _Kp, double _phong_size) : tracer::shader("TexturedShader"), filename(_filename), Ka(_Ka), Kd(_Kd), Kp(_Kp), phong_size(_phong_size) {
	loadBmp(filename);
}

tracer::color TextureShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
	return tracer::black;
}

tracer::color TextureShader::transparency(const tracer::shade_context& context) {
	return tracer::black;
}

void TextureShader::loadRaw(std::string filename, int _width, int _height, int offset) {
	height = _height;
	width = _width;
	data = (char*)malloc(3*width*height);

	FILE *file = fopen(filename.c_str(), "r");
	fseek(file, offset, SEEK_CUR);
	fread(data, 3, width*height, file);
	fclose(file);
}

void TextureShader::loadBmp(std::string filename) {
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	FILE *file = fopen(filename.c_str(), "r");

	fread((void*)&bmfh, sizeof(BITMAPFILEHEADER), 1, file);		
	fread((void*)&bmih, sizeof(BITMAPINFOHEADER), 1, file);		

	fclose(file);

	loadRaw(filename, bmih.biWidth, bmih.biHeight, bmfh.bfOffBits);
}

tracer::color TextureShader::get(double s, double t) {
  int x = (int)(s*width) % width, y = (int)(t*height) % height;

  return tracer::color(data[3*(width*y+x)]/255.0, data[3*(width*y+x)+1]/255.0, data[3*(width*y+x)+2]/255.0);
}

std::ostream& TextureShader::toStream(std::ostream& out) const {
	return out << "filename " << filename << "Ka " << Ka << " Kd " << Kd << " Kp " << Kp << " phong_size " << phong_size;
}

std::istream& TextureShader::fromStream(std::istream& in) {
	std::string tmp = "";

	while (tmp != "}") {
		in >> tmp;

		if (tmp == "filename") {
			in >> filename;
		} else if (tmp == "Ka") {
			in >> Ka;
		} else if (tmp == "Kd") {
			in >> Kd;
		} else if (tmp == "Kp") {
			in >> Kp;
		} else if (tmp == "phong_size") {
			in >> phong_size;
		}
	}

	return in;
}

tracer::shader* TextureShader::clone() {
	return new TextureShader(filename, Ka, Kd, Kp, phong_size);
}
