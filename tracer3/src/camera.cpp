#include "camera.h"

namespace tracer {
  camera::camera(vector _origin) : serializable("camera"), origin(_origin) {
    right = vector(1,0,0);
    up = vector(0,1,0);
    top = vector(0,1,0);
    front = vector(0,0,-1);

    flength = 50;
    fstop = 8;
    shutterSpeed = 1.0/90.0;
    fdistance = 1;
  }
  
  camera::~camera() {
  }

  void camera::lookAt(vector p) {
    //    std::cerr << "p is " << p << ", origin is " << origin << ", front is " << ((p - origin)^1) << ", up is " << ((up - (front*up)*front)^1) << ", right is " << (front.cross(up)^1) << std::endl;
    //std::cerr.flush();

//    std::cerr << "before - origin: " << origin << ", lookat: " << lookat << ", front: " << front << ", up: " << up << ", right: " << right << std::endl;

    front = (p - origin)^1;
    up = (top - (front*top)*front)^1;
    right = front.cross(up)^1;

//    std::cerr << "after - origin: " << origin << ", lookat: " << lookat << ", front: " << front << ", up: " << up << ", right: " << right << std::endl;
  }

  void camera::focusAt(vector p) {
    /** @todo this does not focus on exact point, but on a plane
              that is |origin-p| units from the camera. */
    fdistance = (origin-p).length();
  }
  
  ray camera::createRay(double x, double y, double t, int stochastic) {
    if (stochastic) {
      //time
      t = t + shutterSpeed*((rand()%2000-1000)/1000.0);
    }

    if (!origin_spline.empty() || !lookat_spline.empty()) {
      if (!origin_spline.empty()) {
	origin = origin_spline(t);
      }      

      if (!lookat_spline.empty()) {
	lookat = lookat_spline(t);
      } 

      lookAt(lookat);
      focusAt(lookat);

      // std::cerr << "t: " << t << ", origin: " << origin << ", lookat: " << lookat << ", front: " << front << ", up: " << up << ", right: " << right << std::endl;
    }

    if (!flength_spline.empty()) {
      flength = flength_spline(t);
    }

    if (!fdistance_spline.empty()) {
      fdistance = fdistance_spline(t);
    }

    if (stochastic) {
      // calculate 
      vector v  = ((flength/1000.0)*front + x*right + y*up)^1;
      double apertureArea = (flength/1000.0)/fstop, apertureRadius = ::sqrt(apertureArea/3.14159);
      
      // displace origin
      double xDisp = (rand()%2000-1000)/1000.0, yDisp = (rand()%2000-1000)/1000.0;
      while (xDisp*xDisp + yDisp*yDisp > 1.0) {
	xDisp = (rand()%2000-1000)/1000.0;
	yDisp = (rand()%2000-1000)/1000.0;
      }
      
      // calc new origin and new v
      tracer::vector focPoint = origin + (fdistance/::fabs(v*front))*v;
      tracer::vector newOrg = origin + apertureRadius*(xDisp*right + yDisp*up);
      tracer::vector newV = (focPoint - newOrg)^1;
      
      return tracer::ray(newOrg, newV, t);
    } else {
      return ray(origin, (x*right + y*up + (flength/1000.0)*front)^1, t);
    }
  }

  std::istream& camera::fromStream(std::istream& in) {
    std::string token;

    in >> token;

    while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\r' || in.peek() == '\n') in.get();

    while (token != "}") {
      while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\r' || in.peek() == '\n') in.get();

      if (token == "position" ) {
	if (in.peek() == '{') {
	  in >> origin_spline;
	} else {
	  in >> origin;
	}
      } else if (token == "up") {
	in >> up;
      } else if (token == "look_at") {
	if (in.peek() == '{')  in >> lookat_spline;
	else {
	  in >> lookat;
	  lookAt(lookat);
	}
      } else if (token == "focus_at") {
	vector p;
        in >> p;
        focusAt(p);
      } else if (token == "flength") {
	if (in.peek() == '{') in >> flength_spline;
	else in >> flength;
      } else if (token == "fstop") {
	in >> fstop;
      } else if (token == "shutter_speed") {
	in >> shutterSpeed;
      } else if (token == "fdistance") {
	if (in.peek() == '{') in >> fdistance_spline;
	else in >> fdistance;
      }

      in >> token;
    }

    return in;
  }
  
  std::ostream& camera::toStream(std::ostream& out) const {
    return out << " origin " << origin << " flength " << flength << " fstop " << fstop << " shutter_speed " << shutterSpeed << " fdistance " << fdistance << " ";
  }

  camera* camera::clone() {
    return new camera;
  }
}
