#!/bin/sh

CLASSNAME=$1
CLASSNAMEUPPER=`echo $CLASSNAME | awk '{ print toupper($1) }'`
CLASSNAMELOWER=`echo $CLASSNAME | awk '{ print tolower($1) }'`

echo "#include \"${CLASSNAMELOWER}.h\""
echo 
echo "${CLASSNAME}::${CLASSNAME}() : tracer::shape(\"${CLASSNAME}\") {"
echo "}"
echo
echo "${CLASSNAME}::~${CLASSNAME}() {"
echo "}"
echo
echo "void ${CLASSNAME}::intersect(const tracer::ray& r, tracer::intersection_collection& icoll) {"
echo "}"
echo
echo "int ${CLASSNAME}::inside(const tracer::vector& p, double t) {"
echo "  return 0;"
echo "}"
echo
echo "std::ostream& ${CLASSNAME}::toStream(std::ostream& out) const {"
echo "  return out;"
echo "}"
echo
echo "std::istream& ${CLASSNAME}::fromStream(std::istream& in) {"
echo "  while (in.get() != '}');"
echo
echo "  return in;"
echo "}"
echo
echo "tracer::shape* ${CLASSNAME}::clone() {"
echo "  return new ${CLASSNAME}();"
echo "}"
echo

