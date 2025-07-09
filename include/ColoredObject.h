#ifndef COLOREDOBJECT_H
#define COLOREDOBJECT_H

#include <string>

enum class Color { Red, Green, Blue };

struct ColoredObject {
    Color m_color;
    std::string m_payload;

    ColoredObject(Color c, std::string pl): m_color(c), m_payload(std::move(pl)) {}
};

#endif // COLOREDOBJECT_H
