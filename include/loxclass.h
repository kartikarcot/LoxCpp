#pragma once
#include <string>

class LoxClass {
public:
  std::string name;
  LoxClass(const std::string name) : name(name){};
  std::string to_string() { return name; }
  ~LoxClass(){};
};
