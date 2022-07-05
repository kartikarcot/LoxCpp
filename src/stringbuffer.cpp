#include "stringbuffer.h"

void StringBuffer::init() { ss.clear(); }

void StringBuffer::increase_indent() { indent_++; }
void StringBuffer::decrease_indent() { indent_ = std::max(0, --indent_); }

void StringBuffer::write_line(const char *fmt, ...) {
  std::string indent_str = std::string(indent_ * 2, ' ');
  ss << indent_str;
  va_list args;
  // get all the arguments after the argument fmt in this fn
  va_start(args, fmt);
  vsnprintf(line, 255, fmt, args);
  va_end(args);
  ss << line;
  ss << "\n";
  return;
}

void StringBuffer::get_contents(std::string &contents) {
  contents = ss.str();
  return;
}
