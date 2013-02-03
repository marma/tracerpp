#!/bin/sh

CLASSNAME=$1
CLASSNAMELOWER=`echo ${CLASSNAME} | awk '{ print tolower($1) }'`

if [ $# -ne 1 ]; then
  echo "usage: addlightsource.sh <class name>"
  exit 1
fi

./createlightsource_h.sh ${CLASSNAME} > lightsources/${CLASSNAMELOWER}.h
./createlightsource_cpp.sh ${CLASSNAME} > lightsources/${CLASSNAMELOWER}.cpp

