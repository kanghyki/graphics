#ifndef INCLUDED_UTIL_H
#define INCLUDED_UTIL_H

#include <optional>
#include <string>
#include <vector>

std::optional<std::string> LoadTextFile(const std::string &filename);
std::vector<std::string> Split(const std::string &s, const std::string &sep);

#endif
