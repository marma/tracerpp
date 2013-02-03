#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include <math.h>

namespace tracer {
       class color {
	 public:
	  // constructors and destructors 
	  inline color() { }
	  inline color(double _r, double _g, double _b) : r(_r), g(_g), b(_b) {}

	  // methods
	  inline double value() {
	    return 0.299*r+0.587*g+0.114*b; 
	  }

	  inline color exp(double d=1) {
	    return color(::exp(d*r), ::exp(d*g), ::exp(d*b));
	  }
	  
	  inline color pow(double d) { 
	    return color(::pow(r,d), ::pow(g,d), ::pow(b,d));
	  }
	  
	  inline color abs() {
	    return color(::fabs(r), ::fabs(g), ::fabs(b));
	  }
	  
	  inline color clamp(double min, double max) {
	    color c(*this); 
	    c.r = (r<min)?min:r; c.r = (r>max)?max:r;
	    c.g = (g<min)?min:g; c.g = (g>max)?max:g;
	    c.b = (b<min)?min:b; c.b = (b>max)?max:b; 

	    return c; 
	  }

	  inline color map(double out_min, double out_max, double in_min=0.0, double in_max=1.0) { 
	    double s = (out_max - out_min)/(in_max - in_min);

	    return color((r-in_min)*s + out_min, (g-in_min)*s + out_min, (b - in_min)*s + out_min);
	  }						             

	  // operators
	  color operator + (const color& c) { return color(r+c.r, g+c.g, b+c.b); }
	  color operator - (const color& c) { return color(r-c.r, g-c.g, b-c.b); }
	  color operator * (const color& c) { return color(r*c.r, g*c.g, b*c.b); }
	  color operator * (double d) { return color(r*d, g*d, b*d); }
	  color operator / (double d) { return color(r/d, g/d, b/d); }
	  color& operator += (const color& c) { r+=c.r; g+=c.g; b+=c.b; return *this; }
	  color& operator -= (const color& c) { r-=c.r; g-=c.g; b-=c.b; return *this; }
	  color& operator *= (const color& c) { r*=c.r; g*=c.g; b*=c.b; return *this; }
	  color& operator *= (double d) { r*=d; g*=d; b*=d; return *this; }
	  color& operator /= (double d) { r/=d; g/=d; b/=d; return *this; }
	  //	  int operator == (const color& c) { return c.r == r && c.g == g && c.b == b; }
	  int operator != (const color& c) { return c.r != r || c.g != g || c.b != b; }
	  
	  // friends
	  friend inline color operator * (double d, const color& c) {
		return tracer::color(d*c.r, d*c.g, d*c.b);
	  }

	  friend inline std::ostream& operator << (std::ostream& out, const color& c) {
		return out << '<' << c.r << " " << c.g << " " << c.b << '>';
	  }

	  friend inline std::istream& operator >> (std::istream& in, color& c) {
		  while (in.peek() != '<') in.get();

		  std::string tmp;

		  in.get();
		  in >> c.r >> c.g >> c.b;
		  in.get();

		  return in;
	  }

	  // data members
	  double r,g,b;
	};

        static color black(0,0,0), white(1,1,1), red(1,0,0), green(0,1,0), blue(0,0,1), opaque(0,0,0), transparent(1,1,1);
}

#endif








