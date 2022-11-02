#include "env.h"
#include "spdlog/spdlog.h"
#include "utils.h"

Environment::~Environment() {
  // do nothing here. do not delete the enclosing environment
  // it's not your job
}

void Environment::define(const std::string &name, const Object &value) {
  env_map[name] = value;
}

bool Environment::assign(const std::string &name, const Object &value) {
  if (env_map.find(name) != env_map.end()) {
    env_map[name] = value;
    return true;
  }
  bool ret = false;
  if (enclosing != nullptr) {
    ret = enclosing->assign(name, value);
  }
  if (!ret) {
    report("Variable " + name + " is not defined", "", 0);
    return false;
  }
  return true;
}

Object *Environment::get(const Token &t) {
  spdlog::debug("Fetching value for {}", t.literal_string);
  if (env_map.find(t.literal_string) != env_map.end()) {
    return &env_map.at(t.literal_string);
  }
  Object *o = nullptr;
  if (enclosing != nullptr) {
    o = enclosing->get(t);
  }
  if (o == nullptr) {
    report("Variable " + t.literal_string + " is not defined", "", 0);
    return nullptr;
  }
  return o;
}
