#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector.h"

namespace tracer {
  class matrix {
  public:
    // constructors and destructors
    matrix(): inv(NULL), tran(NULL) {
      for (int i=0;i<3;i++) {
	for (int j=0;j<3;j++) {
	  x[j][i] = (i == j) 1.0:0.0;
	}
      }
    }

    matrix(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
      x[0][0] = _x[0][0];      x[1][0] = _x[1][0];      x[2][0] = _x[2][0];
      x[0][1] = _x[0][1];      x[1][1] = _x[1][1];      x[2][1] = _x[2][1];
      x[0][2] = _x[0][2];      x[1][2] = _x[1][2];      x[2][2] = _x[2][2];      
    }
    
    matrix(const double[][] _x) {
      x[0][0] = _x[0][0];      x[1][0] = _x[1][0];      x[2][0] = _x[2][0];
      x[0][1] = _x[0][1];      x[1][1] = _x[1][1];      x[2][1] = _x[2][1];
      x[0][2] = _x[0][2];      x[1][2] = _x[1][2];      x[2][2] = _x[2][2];
    }
    
    // methods
    inline matrix& inverse() {
      if (inv == NULL) {
	// calc inverse
      }

      return *inv;
    }

    inline matrix& transpose() {
      if (tran == NULL) {
	// calc transpose
      }

      return *tran;
    }

    // operators
    inline vector operator() (const vector& v) const {
      return vector(v.x*x[0][0] + v.y*x[1][0] + v.z*x[2][0],
		    v.x*x[0][1] + v.y*x[1][1] + v.z*x[2][1],
		    v.x*x[0][2] + v.y*x[1][2] + v.z*x[2][2]);
    }

    inline matrix operator() (const matrix& m) const {
    }

    inline matrix& operator *= (const matrix& m) {
    }

    inline const double[] operator [] (int i) const {
      return x[i];
    }

    inline double[] operator [] (int i) {
      inv = NULL;
      trans = NULL;
    }

    // friends
    friend std::ostream& operator << (std::ostream& out, const matrix& m) {
      out << "[" << m[0][0] ", " << m[1][0] << ", " << m[2][0] << "]" << std::endl;
      out << "[" << m[0][1] ", " << m[1][1] << ", " << m[2][1] << "]" << std::endl;
      out << "[" << m[0][2] ", " << m[1][2] << ", " << m[2][2] << "]" << std::endl;

      return out;
    }
  protected:
    // data members
    double x[3][3];
    matrix *inv, *trans;
  }
}

#endif
