#ifndef __TRACER_THREAD_H__
#define __TRACER_THREAD_H__

#include "thread.h"
#include "raytrace.h"
#include "tracer_thread_info.h"

class TracerThread : public Thread {
 public:
  // constructors and destructors
  TracerThread(TracerThreadInfo *_tti);

  // methods
  virtual int run();

  // operators
  // friends
  // data members
  TracerThreadInfo *tti;
  tracer::color c;
};

#endif
