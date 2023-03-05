#pragma once
#include "parser.h"
#include <cstring>
#include <string>
#include <vector>

enum ObjectType { STR, FLOAT, BOOL, FUNCTION, CLASS_TYPE, UNDEFINED };

class Evaluator;

class Object;

class Callable {
public:
  std::string name = "Callable";
  virtual int arity() = 0;
  virtual Object call(std::vector<Object> args, Evaluator *env) = 0;
  virtual ~Callable(){};
  virtual Callable *Clone() = 0;
};

struct Object {
  ObjectType type = UNDEFINED;
  void *val = NULL;
  Object(){};
  Object(ObjectType type, void *val) : type(type), val(val){};
  Object(Object &&o) {
    std::swap(this->val, o.val);
    std::swap(this->type, o.type);
  }

  void copy_data(const Object &o);
  // copy constructor
  Object(const Object &o) { copy_data(o); }

  // move assignment operator
  Object &operator=(Object &&o) {
    std::swap(this->val, o.val);
    std::swap(this->type, o.type);
    return *this;
  }

  // assignment operator
  Object &operator=(const Object &o) {
    this->release_data();
    this->copy_data(o);
    return *this;
  }

  // destructor
  ~Object() { release_data(); }

  void release_data();

  // making this return std string since we dont want to worry about
  // deallocating this. it is mostly used for logging
  static std::string type_to_str(ObjectType type);
  // making this return std string since we dont want to worry about
  // deallocating this. it is mostly used for logging
  static std::string object_to_str(Object val);
};
