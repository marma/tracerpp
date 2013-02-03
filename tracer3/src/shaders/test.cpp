#include "test.h"

Test::Test() : tracer::shader("Test") {
}

tracer::color shade(const tracer::ray& ray, const tracer::shade_context& context) {
  return tracer::white;
}
std::ostream& Test::toStream(std::ostream& out) const {
  return out;
}

std::istream& Test::fromStream(std::istream& in) {
  return in;
}

tracer::shader* Test::clone() {
  return new Test();
}

