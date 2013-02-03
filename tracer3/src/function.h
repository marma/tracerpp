#ifndef __FUNCTION_H__
#define __FUNCTION_H__

template<class T, class X> class function {
 public:
  virtual T operator () (const X& x) =0;
};

#endif
