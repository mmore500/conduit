#pragma once

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

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

template <typename Out>
void read_lines(std::istream & is, Out out) {
    using In = std::istream_iterator<uit::Line>;
    std::copy(In(is), In(), out);
}

};