#!/bin/sh

CLASSNAME=$1
CLASSNAMEUPPER=`echo $CLASSNAME | awk '{ print toupper($1) }'`
CLASSNAMELOWER=`echo $CLASSNAME | awk '{ print tolower($1) }'`

echo "#include \"${CLASSNAMELOWER}.h\""
echo 
echo "${CLASSNAME}::${CLASSNAME}() : tracer::lightsource(\"${CLASSNAME}\") {"
echo "}"
echo
echo "tracer::ray ${CLASSNAME}::shade(const tracer::vector& p, double t) {"
echo "  return tracer::ray(p + tracer::vector(0,10,0), tracer::vector(0,-1,0), t, tracer::white);"
echo "}"
echo
echo "int ${CLASSNAME}::inside(const tracer::vector& p, double t) {"
echo "  return 1;"
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
echo "tracer::lightsource* ${CLASSNAME}::clone() {"
echo "  return new ${CLASSNAME}();"
echo "}"
echo

