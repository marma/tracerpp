#ifndef __LINEAR_SPLINE_H__
#define __LINEAR_SPLINE_H__

#include <vector>
#include "spline.h"
#include "serializable.h"

template<class T> class linear_point {
 public:
  linear_point() {}
  linear_point(const T& _x, const double& _t) : x(_x), t(_t) {}
  T x;
  double t;
};

template<class T> class linear_interval {
 public:
  linear_interval(const linear_point<T>& _p1, const linear_point<T>& _p2) : p1(_p1), p2(_p2), dx(p2.x - p1.x), dt(p2.t - p1.t) {
  }

  T get(double t) {
    //    double x = (t - p1.t)/dt;
    return (t-p1.t)/dt*dx + p1.x;
  }
  
  linear_point<T> p1,p2;
  T dx;
  double dt;
};

template<class T> class linear_spline : public spline<T>, public serializable {
 public:
  linear_spline() : serializable("linear_spline"), last_interval(NULL), min_t(0), max_t(0), loop(0) { }
  
  void add(const T& p, double t) {
    if (points.empty()) {
      min_t = max_t = t;
      points.push_back(linear_point<T>(p,t));
    } else {
      max_t = t;
      linear_point<T> p1 = points[points.size() - 1], p2;
      points.push_back(linear_point<T>(p,t));
      p2 = points[points.size() - 1];

      intervals.push_back(linear_interval<T>(p1,p2));
    }

    //std::cerr << "adding " << p << " at time " << t << std::endl; 
  }

  int empty() {
    return intervals.size() == 0;
  }

  // operators
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

    for (typename std::vector<linear_interval<T> >::iterator iter = intervals.begin(); iter != intervals.end(); iter++) {
      if ((t >= iter->p1.t) && (t <= iter->p2.t)) {
        last_interval = &(*iter);

        return iter->get(t);
      }
    }

    return intervals[intervals.size()-1].get(intervals[intervals.size()-1].p2.t);
  }

  virtual std::ostream& toStream(std::ostream& out) const {
    for (typename std::vector<linear_point<T> >::const_iterator iter = points.begin(); iter != points.end(); iter++) {
      out << iter->x << " " << iter->t << " ";
    }

    return out;
  }

  virtual std::istream& fromStream(std::istream& in) {
    while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\r' || in.peek() == '\n') in.get();

    while (in.peek() != '}') {
      T p;
      double t;

      in >> p >> t;

      add(p,t);

      while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\r' || in.peek() == '\n') in.get();
    }

    in.get();

    return in;
  }
  
  std::vector<linear_point<T> > points;
  std::vector<linear_interval<T> > intervals;
  linear_interval<T> *last_interval;
  double min_t, max_t;
  int loop;
};

#endif
