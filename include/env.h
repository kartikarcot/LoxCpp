#pragma once
#include "object.h"
#include "token.h"
#include <unordered_map>

class Environment {
public:
  std::unordered_map<std::string, Object> env_map;

public:
  void define(const std::string &name, const Object &value);
  Object *get(const Token &t);
};
