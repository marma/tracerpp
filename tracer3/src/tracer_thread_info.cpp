#include "tracer_thread_info.h"

// constructors and destructors
TracerThreadInfo::TracerThreadInfo(tracer::scene *__scene, tracer::camera *__camera, int _width, int _height, double _max, double _flength, double _fstop, double _sspeed, double _fdistance, double _time, int _nSamples) : _scene(__scene), _camera(__camera), width(_width), height(_height), max(_max) ,flength(_flength), fstop(_fstop), sspeed(_sspeed), fdistance(_fdistance), time(_time), nSamples(_nSamples), n(0) {
  data = new tracer::color[width*height];
}

TracerThreadInfo::~TracerThreadInfo() {
  delete[] data;
}

// methods
int TracerThreadInfo::getLine() {
  int ret = n++;
  
  return (ret < height)? ret:-1;
}

void TracerThreadInfo::set(int x, int y, tracer::color c) {
  data[y*width + x] = c;
}

void TracerThreadInfo::save(std::string filename) {
  std::ofstream out(filename.c_str());
  
  tracer::util::writeBmpHeader(width, height, 24, out);
  
  for (int y=0;y<height;y++) {
    for (int x=0;x<width;x++) {
      tracer::util::write8bit(data[y*width + x], out);
    }
  }
  
  out.close();
}

