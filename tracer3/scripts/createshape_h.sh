#!/bin/sh

CLASSNAME=$1
CLASSNAMEUPPER=`echo $CLASSNAME | awk '{ print toupper($1) }'`

echo "#ifndef __${CLASSNAMEUPPER}_H__"
echo "#define __${CLASSNAMEUPPER}_H__"
echo
echo "#include \"../tracer.h\""
echo
echo "class ${CLASSNAME} : public tracer::shape {"
echo "public:"
echo "  // constructors and destructors"
echo "  ${CLASSNAME}();"
echo "  ~${CLASSNAME}();"
echo "  // methods"
echo "  virtual void intersect(const tracer::ray& r, tracer::intersection_collection& icoll);"
echo "  virtual int inside(const tracer::vector& p, double t);"
echo "  virtual std::ostream& toStream(std::ostream& out) const;"
echo "  virtual std::istream& fromStream(std::istream& in);"
echo "  virtual tracer::shape* clone();"
echo "protected:"
echo "  // data members"
echo "};"
echo
echo "#endif"

