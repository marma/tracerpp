#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include <iostream>

template<int degree> class polynomial {
 public:
  polynomial() {
    for (int i=0;i<=degree;i++) {
      constants[i] = 0.0;
    }    
  }

  inline double operator () (double d) {
    double ret = constants[0], x = d;

    for (int i=1;i<=degree;i++) {
      ret += constants[i]*x;
      x *= d;
    }

    return ret;
  }

  inline double& operator [] (int i) {
    return constants[i];
  }

  friend std::ostream& operator << (std::ostream& out, const polynomial<degree>& p) {
    out << p.constants[0];

    for (int i=1;i<=degree;i++) {
      if (p.constants[i] == 0.0) {
	continue;
      } else if (p.constants[i] == 1.0) {
	out << " + ";
      } else {
	out << " + " << p.constants[i] << "*";
      }
	
      if (i == 1) {
	out << "t";
      } else {
	out << "x^" << i;
      }
    }

    return out;
  }

 protected:
  double constants[degree + 1];
}; 

#endif
