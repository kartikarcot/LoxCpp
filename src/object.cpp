#include "object.h"
#include "loxclass.h"
#include "loxfun.h"
// A container that can hold a pointer
// to a value and its type

void Object::copy_data(const Object &o) {
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
    val = ((Callable *)o.val)->Clone();
    break;
  case CLASS_TYPE:
    type = o.type;
    val = o.val;
    break;
  default:
    type = UNDEFINED;
    val = NULL;
    break;
  }
}

void Object::release_data() {
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
  case FUNCTION:
    delete (Callable *)val;
    break;
  case CLASS_TYPE:
    delete (LoxClass *)val;
  default:
    break;
  }
  type = UNDEFINED;
  val = NULL;
}

// making this return std string since we dont want to worry about
// deallocating this. it is mostly used for logging
std::string Object::type_to_str(ObjectType type) {
  char *str = new char[6];
  switch (type) {
  case STR:
    return "STR";
  case FLOAT:
    return "FLOAT";
  case BOOL:
    return "BOOL";
  case FUNCTION:
    return "FUNCTION";
  case CLASS_TYPE:
    return "CLASS_TYPE";
  default:
    return "UNDEFINED";
    break;
  }
  return str;
}

// making this return std string since we dont want to worry about
// deallocating this. it is mostly used for logging
std::string Object::object_to_str(Object val) {
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
    return ((Callable *)val.val)->name;
  case CLASS_TYPE:
    return ((LoxClass *)val.val)->name;
  default:
    return "UNDEFINED";
  }
}
