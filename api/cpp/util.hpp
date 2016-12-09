
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>

namespace util {

// stringify for generics
template <typename T>
static std::string stringify(T v) {
    std::stringstream ss;
    ss << v;
    return ss.str();
}

// stringify for vectors
template <typename T>
static std::string stringify(const std::vector<T> &v) {
    std::stringstream ss;
    ss << "{";
    for (size_t i = 0; i < v.size(); ++i) {
        ss << stringify(v[i]);
        if (i < v.size()-1) {
            ss << ", ";
        }
    }
    ss << "}";
    return ss.str();
}

// equal function for unhandled generics
template <typename T>
static bool equal(T a, T b) {
    return a == b;
}

// equal function for vectors
template <typename T>
static bool equal(const std::vector<T> &a, const std::vector<T> &b) {
    if (!equal(a.size(), b.size())) return false;

    for (size_t i = 0; i < a.size(); ++i) {
        if (!equal(a[i], b[i])) return false;
    }
    return true;
}

// less than function for unhandled generics
template <typename T>
static bool smaller(T a, T b) {
    return a < b;
}

// less than function for unhandled generics
template <typename T>
static bool greater(T a, T b) {
    return a > b;
}

} // util

#endif // UTIL_H
