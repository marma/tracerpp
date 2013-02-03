#!/bin/sh

INFILE=$1
OUTFILE=$2
WIDTH=$3
HEIGHT=$4
FLENGTH=24
FSTOP=1.4
SSPEED=50
FDISTANCE=3.0
SAMPLES=1
T=$5

if [ $# -ne 5 ]; then
  echo "usage: trace.sh <infile> <outfile> <width> <height> <time>"
  exit 1
fi

bin/tracer -m --npasses=1 --flength=$FLENGTH --fstop=$FSTOP --shutter=$SSPEED --fdistance=$FDISTANCE --t=$T --nsamples=$SAMPLES --width=$WIDTH --height=$HEIGHT "--cposition=<1 2 3>" "--clookat=<0 0 0>" $INFILE $OUTFILE

/bin/echo "f-length=$FLENGTH\nf-stop=$FSTOP\nshutter=1/$SSPEED sec.\nf-distance=$FDISTANCE\ntime=$T" > tmp.txt 
convert -font helvetica -fill white -draw 'text 5,14 "@tmp.txt"' $OUTFILE.bmp $OUTFILE.bmp.tmp > /dev/null 2> /dev/null

mv $OUTFILE.bmp.tmp $OUTFILE.bmp > /dev/null 2> /dev/null
rm tmp.txt > /dev/null 2> /dev/null

