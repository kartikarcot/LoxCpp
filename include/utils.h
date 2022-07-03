#pragma once

#include <string>

std::string read_file_into_string(const std::string &file_path);
void report(const std::string &message, const std::string &where, int line_no);
void error(const std::string &message, int line_no);
