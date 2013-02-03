#!/bin/sh

CLASSNAME=$1
CLASSNAMEUPPER=`echo $CLASSNAME | awk '{ print toupper($1) }'`
CLASSNAMELOWER=`echo $CLASSNAME | awk '{ print tolower($1) }'`

echo "#include \"${CLASSNAMELOWER}.h\""
echo 
echo "${CLASSNAME}::${CLASSNAME}() : tracer::volumeshader(\"${CLASSNAME}\") {"
echo "}"
echo "${CLASSNAME}::~${CLASSNAME}() {"
echo "}"
echo
echo "tracer::volumeshader::volume_element ${CLASSNAME}::shade(const tracer::ray& r, const tracer::shade_context& context) {"
echo "  return tracer::volumeshader::volume_element(tracer::white, tracer::opaque, 1.0);"
echo "}"  
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
echo "tracer::volumeshader* ${CLASSNAME}::clone() {"
echo "  return new ${CLASSNAME}();"
echo "}"
echo

