#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <math.h>

class vector {
 public:
  enum { X=0, Y=1, Z=2 };
  // constructors and destructors
  vector() : x(0), y(0), z(0) {}
  vector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
  // methods
  // operators
  vector operator - () const { return vector(-x,-y,-z); }
  vector operator + (const vector& v) const { return vector(x+v.x, y+v.y, z+v.z); }
  vector operator - (const vector& v) const { return vector(x-v.x, y-v.y, z-v.z); }
  double operator * (const vector& p) const { return x*p.x + y*p.y + z*p.z; }
  vector operator ^ (double d) const { double s = sqrt(x*x + y*y + z*z); return vector(x*d/s, y*d/s, z*d/s); }
  vector operator ^ (const vector& p) const { return vector(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);}
  vector& operator += (const vector& p) { x+=p.x; y+=p.y; z+=p.z; return *this;}
  vector& operator -= (const vector& p) { x-=p.x; y-=p.y; z-=p.z; return *this;}
  vector& operator *= (double d) { x*=d; y*=d; z*=d; return *this;}
  double length() const { return sqrt(x*x + y*y + z*z); }
  double length2() const { return x*x + y*y + z*z; }
  vector cross(vector v) const { return vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
  //	  vector rotate(double a,double b) { double sina=sin(a),cosa=cos(a),sinb=sin(b),cosb=cos(b); return vector(x*cosa*cosb - y*sina + z*cosa*sinb, x*sina*cosb + y*cosa + z*sina*sinb, -x*sinb + z*cosb); }
  vector rotate(vector axis, double theta) {
    vector q(0,0,0);
    double costheta,sintheta;
    
    axis = axis^1;
    
    costheta = cos(theta);
    sintheta = sin(theta);
    
    q.x += (costheta + (1 - costheta) * axis.x * axis.x) * x;
    q.x += ((1 - costheta) * axis.x * axis.y - axis.z * sintheta) * y;
    q.x += ((1 - costheta) * axis.x * axis.z + axis.y * sintheta) * z;
    
    q.y += ((1 - costheta) * axis.x * axis.y + axis.z * sintheta) * x;
    q.y += (costheta + (1 - costheta) * axis.y * axis.y) * y;
    q.y += ((1 - costheta) * axis.y * axis.z - axis.x * sintheta) * z;
    
    q.z += ((1 - costheta) * axis.x * axis.z - axis.y * sintheta) * x;
    q.z += ((1 - costheta) * axis.y * axis.z + axis.x * sintheta) * y;
    q.z += (costheta + (1 - costheta) * axis.z * axis.z) * z;
    
    return(q);
  }
  
  
  // friends
  friend inline vector operator * (double d, const vector& v) {
    return vector(d*v.x, d*v.y, d*v.z);
  }
  
  friend inline std::ostream& operator << (std::ostream& out, const vector& x) {
    return out << '<' << x.x << " " << x.y << " " << x.z << '>';
  }
  
  friend inline std::istream& operator >> (std::istream& in, vector& x) {
    while (in.peek() != '<') in.get();
    in.get();
    
    in >> x.x >> x.y >> x.z;
    
    in.get();
    
    return in;
  }
  
  // data members
  double x,y,z;
};

#endif

