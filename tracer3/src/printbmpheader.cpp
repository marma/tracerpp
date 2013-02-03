#include <iostream>
#include "utils.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "usage: printbmpheader <bmpfile>" << std::endl;
    std::exit(1);
  }

  tracer::util::printBmpHeader(std::string(argv[1]), std::cout);

  return 0;
}
