#include "env.h"
#include "logger.h"
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

std::string Environment::print() {
  // print the key value store of the environment
  // and then print the enclosing environment
  // if it exists
  std::string ret = "";
  for (auto &kv : env_map) {
    ret += kv.first + " = " + Object::object_to_str(kv.second) + "\n";
  }
  if (enclosing != nullptr) {
    ret += "Enclosing environment:\n";
    ret += enclosing->print();
  }
  return ret;
}

Object *Environment::get_at(int distance, const std::string &name) {
  // get the environment at the given distance
  // and then get the value from that environment
  Environment *e = this;
  for (int i = 0; i < distance; i++) {
    if (e->enclosing == nullptr) {
      CLog::FLog(LogLevel::ERROR, LogCategory::ENV,
                 "[get_at] Enclosing environment is null");
      return nullptr;
    }
    e = e->enclosing;
  }
  return e->get(name);
}

bool Environment::assign_at(int distance, const std::string &name,
                            const Object &value) {
  // get the environment at the given distance
  // and then assign the value to that environment
  Environment *e = this;
  for (int i = 0; i < distance; i++) {
    if (e->enclosing == nullptr) {
      CLog::FLog(LogLevel::ERROR, LogCategory::ENV,
                 "[assign_at] Enclosing environment is null");
      return false;
    }
    e = e->enclosing;
  }
  e->define(name, value);
  return true;
}
