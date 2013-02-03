#include "vector.h"
#include "hermitespline.h"

int main(int argc, char *argv[]) {
  hermite_spline<tracer::vector> hs;

  std::cin >> hs;

  for (double t=0.0; t<=4; t += 0.01) {
    tracer::vector p = hs(t);

    std::cout << p.x << " " << p.y << std::endl;
  }

  std::cerr << hs << std::endl;

  return 0;
}
