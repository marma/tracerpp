#!/bin/sh

CLASSNAME=$1
CLASSNAMELOWER=`echo ${CLASSNAME} | awk '{ print tolower($1) }'`

if [ $# -ne 1 ]; then
  echo "usage: addvolumeshader.sh <class name>"
  exit 1
fi

./createvolumeshader_h.sh ${CLASSNAME} > volumeshaders/${CLASSNAMELOWER}.h
./createvolumeshader_cpp.sh ${CLASSNAME} > volumeshaders/${CLASSNAMELOWER}.cpp

