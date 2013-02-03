#include <iostream>
#include "bernstein.h"

int main(int argc, char *argv[]) {
  bernstein_polynomial<3> B;

  std::cout << B << std::endl;
}
