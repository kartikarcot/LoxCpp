#pragma once
#include "parser.h"
#include <cstring>
#include <string>
#include <vector>

enum ObjectType { STR, FLOAT, BOOL, FUNCTION, UNDEFINED };

// A container that can hold a pointer
// to a value and its type
struct Object {
  ObjectType type = UNDEFINED;
  void *val = NULL;
  Object(){};
  Object(ObjectType type, void *val) : type(type), val(val){};
  Object(Object &&o) {
    std::swap(this->val, o.val);
    std::swap(this->type, o.type);
  }

  void copy_data(const Object &o) {
    switch (o.type) {
    case STR:
      type = o.type;
      val = new char[strlen((char *)o.val) + 1];
      strcpy((char *)val, (char *)o.val);
      break;
    case FLOAT:
      type = o.type;
      val = new float(*(float *)o.val);
      break;
    case BOOL:
      type = o.type;
      val = new bool(*(bool *)o.val);
      break;
    case FUNCTION:
      type = o.type;
      val = o.val;
      break;
    default:
      type = UNDEFINED;
      val = NULL;
      break;
    }
  }

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

  void release_data() {
    switch (type) {
    case STR:
      delete (char *)val;
      break;
    case FLOAT:
      delete (float *)val;
      break;
    case BOOL:
      delete (bool *)val;
      break;
    default:
      break;
    }
    type = UNDEFINED;
    val = NULL;
  }

  // making this return std string since we dont want to worry about
  // deallocating this. it is mostly used for logging
  static std::string type_to_str(ObjectType type) {
    char *str = new char[6];
    switch (type) {
    case STR:
      return "STR";
    case FLOAT:
      return "FLOAT";
    case BOOL:
      return "BOOL";
    default:
      return "UNDEFINED";
      break;
    }
    return str;
  }

  // making this return std string since we dont want to worry about
  // deallocating this. it is mostly used for logging
  static std::string object_to_str(Object val) {
    switch (val.type) {
    case STR:
      return std::string((char *)val.val);
    case FLOAT:
      return std::to_string(*(float *)val.val);
    case BOOL:
      if (*(bool *)val.val)
        return "true";
      else
        return "false";
    case FUNCTION:
      return "FUNCTION";
    default:
      return "UNDEFINED";
    }
  }
};

class Evaluator;

class Callable {
public:
  virtual int arity() = 0;
  virtual Object call(std::vector<Object> args, Evaluator *env) = 0;
  virtual ~Callable(){};
};
