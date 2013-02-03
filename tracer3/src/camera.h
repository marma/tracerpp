#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "tracer.h"
#include "math/linear_spline.h"

namespace tracer {
  class camera : public serializable {
  public:
    // contructors and destructors
    camera(vector _origin = vector(0,0,0));    
    ~camera();

    // methods
    ray createRay(double x, double y, double t, int stochastic = 0);
    void lookAt(vector p);
    void focusAt(vector p);
    virtual std::istream& fromStream(std::istream& in);
    virtual std::ostream& toStream(std::ostream& out) const;
    virtual camera *clone();

    // friends
    // data members
    vector origin, top, up, front, right, lookat;
    double flength, fstop, shutterSpeed, fdistance;
    linear_spline<tracer::vector> origin_spline, lookat_spline;
    linear_spline<double> flength_spline, fstop_spline, shutterSpeed_spline, fdistance_spline;
  };
}

#endif
