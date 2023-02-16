#include "utils.h"
#include "logger.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
std::string read_file_into_string(const std::string &file_path) {
  std::string file_contents;
  if (!fs::exists(file_path)) {
    return "";
  }
  std::ifstream ifs(file_path, std::ifstream::in);
  std::string line;
  std::string contents;
  while (std::getline(ifs, line)) {
    contents = contents + line + "\n";
  }
  return contents;
}

void report(const std::string &message, const std::string &where, int line_no) {
  CLog::FLog(LogLevel::ERROR, LogCategory::ALL, "[line: %d] Error %s: %s",
             line_no, where.c_str(), message.c_str());
}

void error(const std::string &message, int line_no) {
  report(message, "", line_no);
}

std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

// trim from start (in place)
void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// trim from end (in place)
void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}
