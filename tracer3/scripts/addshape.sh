#!/bin/sh

CLASSNAME=$1
CLASSNAMELOWER=`echo ${CLASSNAME} | awk '{ print tolower($1) }'`

if [ $# -ne 1 ]; then
  echo "usage: addshape.sh <class name>"
  exit 1
fi

./createshape_h.sh ${CLASSNAME} > shapes/${CLASSNAMELOWER}.h
./createshape_cpp.sh ${CLASSNAME} > shapes/${CLASSNAMELOWER}.cpp

