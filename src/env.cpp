#include "env.h"
#include "spdlog/spdlog.h"
#include "utils.h"

void Environment::define(const std::string &name, const Object &value) {
  env_map[name] = value;
}

void Environment::assign(const std::string &name, const Object &value) {
  if (env_map.find(name) == env_map.end()) {
    report("Variable " + name + " is not defined", "", 0);
    return;
  }
  env_map[name] = value;
  return;
}

Object *Environment::get(const Token &t) {
  if (env_map.find(t.literal_string) == env_map.end()) {
    report("Variable " + t.literal_string + " is not defined", "", 0);
    return nullptr;
  }
  return &env_map.at(t.literal_string);
}
