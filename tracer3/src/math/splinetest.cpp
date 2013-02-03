#include "spline.h"
#include "vector.h"
#include "vector_spline.h"
#include "hermite_spline.h"
#include "polynomial.h"
#include "linear_spline.h"

class A : vector_spline {
public:
  A() {}

  vector operator () (const double& x) {
    return vector(1,1,1);
  }
};

int main(int argc, char *argv[]) {
  linear_spline<vector> f;

  /*  
  f.add(vector(0,0,0), 0.0);
  f.add(vector(1,0,0), 0.5);
  f.add(vector(1.5, 0, 0), 1.0);
  */

  std::cin >> f;

  std::cout << f << std::endl;

  for (double x=0.0; x <= 1.0; x += 0.1) {
    std::cout << "f(" << x << ") = " << f(x) << std::endl;
  }

  return 0;
}
