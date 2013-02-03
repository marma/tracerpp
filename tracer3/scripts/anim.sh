#!/bin/sh

INFILE=$1
OUTFILE=$2
WIDTH=$3
HEIGHT=$4
MAXPROCESSES=$5
FRAMES=25
INCREMENT=0.04
T0=0.0

N=00
T=$T0
PROC=0

if [ $# -ne 5 ]; then
  echo "usage: anim.sh <infile> <outfile> <width> <height> <number of processes>"
  exit 1
fi

while true
do   
  echo frame $N ...
  ./trace.sh $INFILE $OUTFILE.$N $WIDTH $HEIGHT $T > /dev/null 2> /dev/null &

  PROC=`expr $PROC + 1`

  if [ $PROC -eq $MAXPROCESSES ]; then
    wait
    PROC=0
  fi

  N=`expr $N + 1`
  T=`echo "$T $INCREMENT + p" | dc`

  if [ $N -eq $FRAMES ]; then     
    break;   
  fi   

  if [ $N -lt 10 ]; then
    N=0$N
  fi

  if [ $PROC -eq $MAXPROCESSES ]; then
    wait
    
  fi
done

