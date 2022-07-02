#include "stdio.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void run(std::string lox_code) {
	spdlog::info("The contents are\n{0}",lox_code);
}

std::string read_file_into_string(std::string file_path) {
	std::string file_contents;
	if (!fs::exists(file_path)) {
		return "";
	}
	std::ifstream ifs(file_path, std::ifstream::in); 
	std::string line;
	std::string contents;
	while (std::getline(ifs, line)) {
		spdlog::info("{0}", line);
		contents = contents + line + "\n";
	}
	return contents;
}

void run_file(std::string file_path) {
	std::string contents = read_file_into_string(file_path);
	run(contents);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		spdlog::info("Interactive mode not implemented yet");
		return -1;
	} else if (argc == 2) {
		std::string file_path = std::string(argv[1]);
		run_file(file_path);
	}else {
		spdlog::info("The corrent usage is cpplox [script] or just cpplox");
		return -1;
	}
	return 0;
}
