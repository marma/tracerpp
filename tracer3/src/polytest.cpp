#include <iostream>
#include "polynomial.h"

int main(int argc, char *argv[]) {
  polynomial<3> f;

  f[0] = 1;
  f[1] = -3.0;
  f[2] = 3.0;
  f[3] = -1.0;

  std::cout << "# f(x) = " << f << std::endl;

  for (double t=0.0; t<=1.0; t += 0.01) {
    double y = f(t);

    std::cout << t << " " << y << std::endl;
  }
}
