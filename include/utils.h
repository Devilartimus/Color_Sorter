#ifndef UTILS_H
#define UTILS_H

#include "ColoredObject.h"
#include <string>
#include <array>
#include <vector>
#include <stdexcept>
#include <unordered_map>

Color stringToColor(const std::string& s);

std::string colorToString(Color c);

std::array<Color, 3> parseRule(const std::string& rule_str);

ColoredObject parseLine(const std::string& line);

bool isSortedCorrectly(const std::vector<ColoredObject>& objects, const std::array<Color, 3>& rule);

#endif // UTILS_H
