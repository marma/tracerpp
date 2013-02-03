#ifndef __TRACER_THREAD_INFO_H__
#define __TRACER_THREAD_INFO_H__

#include <fstream>
#include <string>
#include "tracer.h"
#include "utils.h"
#include "camera.h"

class TracerThreadInfo {
 public:
  // constructors and destructors
  TracerThreadInfo(tracer::scene *__scene, tracer::camera *__camera, int _width, int _height, double _max, double _flength, double _aperture, double _sspeed, double _fdistance, double _time, int _nSamples);
  ~TracerThreadInfo();

  // methods
  int getLine();
  void set(int x, int y, tracer::color c);
  void save(std::string filename);

  // operators
  // friends
  // data members
  tracer::scene *_scene;
  tracer::camera *_camera;
  int width, height;
  double max, flength, fstop, sspeed, fdistance, time;
  int nSamples;
  volatile int n;
  tracer::color *data;
};

#endif
