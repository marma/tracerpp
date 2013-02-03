#!/bin/sh

CLASSNAME=$1
CLASSNAMEUPPER=`echo $CLASSNAME | awk '{ print toupper($1) }'`

echo "#ifndef __${CLASSNAMEUPPER}_H__"
echo "#define __${CLASSNAMEUPPER}_H__"
echo
echo "#include \"../tracer.h\""
echo
echo "class ${CLASSNAME} : public tracer::volumeshader {"
echo "public:"
echo "  // constructors and destructors"
echo "  ${CLASSNAME}();"
echo "  virtual ~${CLASSNAME}();"
echo "  // methods"
echo "  virtual tracer::volumeshader::volume_element shade(const tracer::ray& r, const tracer::shade_context& context);"
echo "  virtual std::ostream& toStream(std::ostream& out) const;"
echo "  virtual std::istream& fromStream(std::istream& in);"
echo "  virtual tracer::volumeshader* clone();"
echo "protected:"
echo "  // data members"
echo "};"
echo
echo "#endif"

