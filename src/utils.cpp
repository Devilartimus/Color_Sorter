#include "../include/utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

/**
 * @brief stringToColor
 * @param s - color flag from input file
 * @return programm color
 * Converting colors (Russian and English in short and full forms)
 */
Color stringToColor(const std::string& s)
{
    static const std::unordered_map<std::string, Color> mapping = {

        {"К", Color::Red}, {"к", Color::Red},
        {"З", Color::Green}, {"з", Color::Green},
        {"С", Color::Blue}, {"с", Color::Blue},

        {"R", Color::Red}, {"r", Color::Red},
        {"G", Color::Green}, {"g", Color::Green},
        {"B", Color::Blue}, {"b", Color::Blue},

        {"Красный", Color::Red}, {"Зеленый", Color::Green}, {"Синий", Color::Blue},
        {"Red", Color::Red}, {"Green", Color::Green}, {"Blue", Color::Blue}
    };

    if (auto it = mapping.find(s); it != mapping.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid color string: " + s);
}

std::string colorToString(Color c)
{
    switch (c) {
    case Color::Red: return "К";
    case Color::Green: return "З";
    case Color::Blue: return "С";
    default: return "?";
    }
}

/**
 * @brief parseRule
 * @param rule_str - input of sorting rule
 * @return programm sorting rule
 */
std::array<Color, 3> parseRule(const std::string& rule_str)
{
    std::istringstream iss(rule_str);
    std::string token;
    std::array<Color, 3> result;
    int index = 0;

    while (std::getline(iss, token, '<') && index < 3) {
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

        if (token.empty()) continue;
        result[index++] = stringToColor(token);
    }

    if (index != 3) {
        throw std::invalid_argument("Rule must contain exactly 3 colors");
    }
    return result;
}

ColoredObject parseLine(const std::string& line)
{
    size_t pos = line.find(';');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid line format: missing ';' in " + line);
    }

    std::string color_str = line.substr(0, pos);
    std::string payload = line.substr(pos + 1);
    return {stringToColor(color_str), payload};
}

bool isSortedCorrectly(const std::vector<ColoredObject>& objects, const std::array<Color, 3>& rule)
{
    if (objects.empty()) return true;

    size_t current_color_index = 0;
    Color current_color = rule[current_color_index];

    for (const auto& obj : objects) {
        if (obj.m_color == current_color) {
            continue;
        }
        else if (current_color_index < 2 && obj.m_color == rule[current_color_index + 1]) {
            current_color_index++;
            current_color = rule[current_color_index];
        }
        else {
            return false;
        }
    }
    return true;
}
