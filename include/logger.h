#pragma once
#include "stdarg.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Ordering is important here
// the first one is the most priority
enum class LogLevel { ERROR, INFO, DEBUG };

namespace LogCategory {
// A list of constexpr char* for categories
const constexpr char *SCANNER = "SCANNER";
const constexpr char *PARSER = "PARSER";
const constexpr char *EVAL = "EVAL";
const constexpr char *ENV = "ENV";
const constexpr char *ALL = "ALL";
const constexpr char *FUN = "FUN";
} // namespace LogCategory

class Logger {
  LogLevel level_ = LogLevel::INFO;
  std::ostream &out_;
  const char *category_ = LogCategory::ALL;

public:
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;
  Logger() = delete;

  Logger(std::ostream &out) : out_(out) {
    // Read the level from the environment
    const char *level = std::getenv("LOG_LEVEL");
    if (level != nullptr) {
      if (std::string(level) == "DEBUG") {
        level_ = LogLevel::DEBUG;
      } else {
        level_ = LogLevel::INFO;
      }
    }
    // Read the category from the environment
    const char *category = std::getenv("LOG_CATEGORY");
    if (category != nullptr && strlen(category) > 0) {
      category_ = category;
    } else {
      category_ = LogCategory::ALL;
    }
  }

  template <typename... Args>
  void Log(LogLevel level, const char *category, Args... args) {
    if (level <= level_ && ((strcmp(category_, LogCategory::ALL) == 0) ||
                            strcmp(category_, category) == 0)) {
      out_ << "[" << std::string(category_) << "] ";
      LogImpl(args...);
    }
  }

private:
  template <typename T, typename... Args> void LogImpl(T t, Args... args) {
    out_ << t << " ";
    LogImpl(args...);
  }

  void LogImpl() { out_ << std::endl; }
};

class CLog;
// A simple singleton logger with cout as the output stream
class CLog {
  inline static Logger *instance;
  CLog() = delete;

public:
  template <typename... Args>
  static void Log(LogLevel level, const char *category, Args... args) {
    if (instance == nullptr) {
      instance = new Logger(std::cout);
    }
    instance->Log(level, category, args...);
  }

  static void FLog(LogLevel level, const char *category, const char *fmt, ...)
      __attribute__((format(printf, 3, 4))) {
    va_list args;
    int size = 2048;
    char line[size];
    // get all the arguments after the argument fmt in this fn
    va_start(args, fmt);
    vsnprintf(line, size, fmt, args);
    va_end(args);
    Log(level, category, line);
  }
};
