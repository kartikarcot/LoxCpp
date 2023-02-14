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
    return false;
  }
  return true;
}

Object *Environment::get(const Token &t) { return get(t.literal_string); }

Object *Environment::get(const std::string &name) {
  if (env_map.find(name) != env_map.end()) {
    return &env_map.at(name);
  }
  Object *o = nullptr;
  if (enclosing != nullptr) {
    o = enclosing->get(name);
  }
  if (o == nullptr) {
    return nullptr;
  }
  return o;
}
