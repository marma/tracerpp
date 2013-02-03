#!/bin/sh

CLASSNAME=$1
CLASSNAMELOWER=`echo ${CLASSNAME} | awk '{ print tolower($1) }'`

if [ $# -ne 1 ]; then
  echo "usage: addshader.sh <class name>"
  exit 1
fi

./createshader_h.sh ${CLASSNAME} > shaders/${CLASSNAMELOWER}.h
./createshader_cpp.sh ${CLASSNAME} > shaders/${CLASSNAMELOWER}.cpp

