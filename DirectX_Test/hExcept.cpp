#include "hExcept.h"
#include <sstream>

hExcept::hExcept(int line, const char* file) noexcept
	: line(line), file(file){}

const char* hExcept::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* hExcept::GetType() const noexcept {
	return "My Exception";
}

int hExcept::GetLine() const noexcept {
	return line;
}

const std::string& hExcept::GetFile() const noexcept {
	return file;
}

std::string hExcept::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

