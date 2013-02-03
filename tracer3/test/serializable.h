#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__

#include <iostream>
#include <string>
#include <map>

class serializable {
 public:
  // constructors and destructors
  serializable(std::string _name);

  // methods
  addHook(std::string name, serializable *x);
  void set(std::string name, 

  // operators
  

  // friends

  // data members
 protected:
  std::string name;
  std::map<std::string, serializable*> hooks;
};

#endif
