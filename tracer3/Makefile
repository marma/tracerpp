CC=g++
#CCFLAGS=-O3 -Wall -pedantic -ansi -march=pentium2 -mmmx
#CCFLAGS=-O3 -Wall -pedantic -ansi -march=pentium4 -msse2 -mfpmath=sse
#CCFLAGS=-O3 -Wall -pedantic -ansi -mcpu=v9
CCFLAGS=-O3 -Wall -pedantic -ansi -D__unix=1
STATIC=

.cpp.o:
	$(CC) $(CCFLAGS) -c $< -o $@

TRACERSRC = raytrace2.cpp camera.cpp tracer.cpp utils.cpp noise.cpp \
            lightsources/simplearealightsource.cpp lightsource_factory.cpp \
            lightsources/simplelightsource.cpp lightsources/stringlight.cpp shaders/noiseshader.cpp \
            shaders/phongshader.cpp shaders/textureshader.cpp shapes/plane.cpp shapes/sphere.cpp \
            shapes/mesh.cpp shaders/mirrorshader.cpp shape_factory.cpp shader_factory.cpp \
            shaders/checkershader.cpp shapes/csg.cpp shapes/aabox.cpp shaders/glassshader.cpp \
            lightsources/skylightsource.cpp shaders/chromerust.cpp shaders/clearshader.cpp \
            shaders/normalshader.cpp lightsources/skysphere.cpp shaders/flatshader.cpp \
	    imager.cpp	

TRACEROBJS = raytrace2.o camera.o tracer.o utils.o noise.o lightsources/simplearealightsource.o \
             lightsources/simplelightsource.o lightsources/stringlight.o shaders/noiseshader.o \
             shaders/phongshader.o shaders/textureshader.o shapes/plane.o shapes/sphere.o \
             shapes/mesh.o shaders/mirrorshader.o lightsource_factory.o shape_factory.o \
             shaders/checkershader.o shapes/csg.o shapes/aabox.o shaders/glassshader.o \
             lightsources/skylightsource.o shaders/chromerust.o shaders/clearshader.o \
             shaders/normalshader.o shader_factory.o lightsources/skysphere.o \
             shaders/flatshader.o imager.o

THREADOBJS = thread.o tracer_thread.o tracer_thread_info.o

SOCKETSRC = clientsocket.cpp serversocket.cpp

SOCKETOBJS = clientsocket.o serversocket.o

tracer: main3.o $(TRACEROBJS)
	$(CC) $(STATIC) -o bin/tracer main3.o $(TRACEROBJS)

nettracer: nettracer.o $(TRACEROBJS) $(SOCKETOBJS)
	$(CC) $(STATIC) -o bin/nettracer nettracer.o $(TRACEROBJS) $(SOCKETOBJS)

threaded_tracer: threaded_main.o $(TRACEROBJS) $(THREADOBJS)
	$(CC) $(STATIC) -o bin/threaded_tracer -lpthread threaded_main.o $(TRACEROBJS) $(THREADOBJS)

printbmpheader: printbmpheader.o utils.o
	$(CC) -o bin/printbmpheader printbmpheader.o utils.o

clean:
	rm -f $(TRACEROBJS) $(THREADOBJS) $(SOCKETOBJS) nettracer.o main3.o threaded_main.o main3.o printbmpheader.o bin/tracer bin/printbmpheader bin/tracer2 bin/threaded_tracer core
	rm Makefile.depend
	touch Makefile.depend

depend:
	$(CC) -M $(TRACERSRC) $(SOCKETSRC) nettracer.cpp main3.cpp > Makefile.depend

include Makefile.depend
