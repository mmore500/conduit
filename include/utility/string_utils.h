#pragma once

#include <string>
#include <iostream>
#include <iterator>

namespace uit {

class Line {
    std::string str;
public:
    Line() = default;

    operator std::string() const { return str; }
    friend std::istream& operator>>(std::istream&, Line&);
};

std::istream& operator>>(std::istream& is, Line& line) {
    std::getline(is, line.str);
    return is;
}

};