#ifndef __BERNSTEIN_H__
#define __BERNSTEIN_H__

#include <iostream>
#include "polynomial.h"

template<int N> class bernstein_polynomial {
public:
  bernstein_polynomial() {
    for (int i=0;i<=N;i++) {
      double c = binomial(N, i);

      for (int j=i;j<=N;j++) {
	double d = binomial(N-i, j-i);

	polynomials[i][j] = c*d*pow(-1, j-i);
      }
    }
  }
  
  polynomial<N>& operator [] (int i) {
    return polynomials[i];
  }

  friend std::ostream& operator << (std::ostream& out, const bernstein_polynomial<N>& B) {
    for (int i=0;i<=N;i++) {
      out << "B[" << i << "] = " << B.polynomials[i] << std::endl;
    }
    
    return out;
  }

  static int binomial(int n, int i) {
    int a = 1,b = 1,c = 1;

    int res=1;
    for (int x=1;x<=n;x++) {
      res *= x;

      if (x == i) a = res;
      if (x == (n-i)) b = res;
      if (x == n) c = res;
    }

    return c/(a*b);
  }
  
protected:
  polynomial<N> polynomials[N+1];
};

#endif
