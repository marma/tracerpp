#ifndef __HERMITESPLINE_H__
#define __HERMITESPLINE_H__

#include <vector>
#include <sstream>
#include "spline.h"
#include "bernstein.h"
#include "serializable.h"

template<class T> class bezier_point {
 public:
  bezier_point() { }
  bezier_point(const T& _p, const double& _t) : p(_p), t(_t) { }
  T p;
  double t;
};

template<class T> class bezier_interval {
 public:
  bezier_interval(const bezier_point<T>& _p1, const bezier_point<T>& _p2) : p1(_p1), p2(_p2) {
    length = fabs(p2.t - p1.t);
  }
  
  T get(double t) {
    double x = (t - p1.t)/length;
    return B[0](x)*p1.p + B[1](x)*(p1.p + length/3.0*p1.v) + B[2](x)*(p2.p - length/3.0*p2.v) + B[3](x)*p2.p;
  }
  
  bernstein_polynomial<3> B;
  hermite_point<T> p1, p2;
  double length;
};

template<class T> class hermite_spline : public spline<T>, public serializable {
 public:
  hermite_spline() : serializable("hermite_spline"), last_interval(NULL), loop(0) {
  }
  
  void add(const T& p, const T& v, double t) {
    if (points.empty()) {
      min_t = t;
      points.push_back(hermite_point<T>(p,v,t));
    } else {
      max_t = t;
      hermite_point<T> p1 = points[points.size() - 1], p2;
      points.push_back(hermite_point<T>(p,v,t));
      p2 = points[points.size() - 1];
      
      //std::cout << "pushing interval " << p1.p << ", " << p2.p << std::endl;
      
      intervals.push_back(hermite_interval<T>(p1,p2));
    }
  }
  
  virtual std::ostream& toStream(std::ostream& out) const {
    for (typename std::vector<hermite_point<T> >::const_iterator iter = points.begin(); iter < points.end(); iter++) {
      out << " point " << iter->p << " " << iter->v << " " << iter->t;
    }

    if (loop) {
      out << " loop ";
    }
    
    return out;
  }

  virtual std::istream& fromStream(std::istream& in) {
    std::string tmp;
    T p,v;
    double t;

    while (tmp != "}") {
      in >> tmp;

      if (tmp == "point") {
	in >> p;
	in >> v;
	in >> t;

	add(p,v,t);
      } else if (tmp == "loop") {
	loop = 1;
      }
    }

    return in;
  }
  
  virtual T operator () (const double& x) {
    double t = x;

    if (loop && (t < min_t || t > max_t)) {
      t -= (max_t - min_t)*floor(t/(max_t - min_t));
    } else {
      if (t < min_t) {
	return intervals[0].get(t);
      } else if (t > max_t) {
	return intervals[intervals.size()-1].get(intervals[intervals.size()-1].p2.t);
      }
    }

    if (last_interval != NULL) {
      if ((t >= last_interval->p1.t) && (t <= last_interval->p2.t)) {
	return last_interval->get(t);
      }
    }
    
    for (typename std::vector<hermite_interval<T> >::iterator iter = intervals.begin(); iter < intervals.end(); iter++) {
      if ((t >= iter->p1.t) && (t <= iter->p2.t)) {
	last_interval = &(*iter);
	
	return iter->get(t);
      }
    }

    return intervals[intervals.size()-1].get(intervals[intervals.size()-1].p2.t);
  }
  
 protected:
  std::vector<hermite_point<T> > points;
  std::vector<hermite_interval<T> > intervals;
  hermite_interval<T> *last_interval;
  double min_t, max_t;
  int loop;
};


#endif
