#ifndef __VECTOR_FUNCTION_H__
#define __VECTOR_FUNCTION_H__

#include "function.h"
#include "vector.h"

template<class X> class vector_function : public function<vector,X> {
 public:
  virtual vector operator () (const X& x) =0;
};

#endif
