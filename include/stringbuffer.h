#include <sstream>
#include <string>

class StringBuffer {
  std::stringstream ss;
  int indent_ = 0;
  constexpr static size_t line_size = 1024;
  char line[line_size];

public:
  void init();
  void increase_indent();
  void decrease_indent();
  void write_line(const char *fmt, ...) __attribute__((format(printf, 2, 3)));
  void get_contents(std::string &contents);
};
