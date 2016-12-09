#ifndef TUNA_H
#define TUNA_H

#include <string>
#include <sstream>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

class API {
private:
    int fd;

public:

    API() {
        init();
    }

    ~API() {
        destroy();
    }

    static const char *pipeName() {
        return "/tmp/tuna";
    }

    void init() {
        mkfifo(pipeName(), 0666);
        fd = open(pipeName(), O_WRONLY);
    }

    void destroy() {
        close(fd);
        unlink(pipeName());
    }

    void send(std::string s) {
        write(fd, s.c_str(), sizeof(char)*s.size());
    }

    template <typename T>
    static std::string compose(T arg) {
        return util::stringify(arg) + "\n";
    }

    template <typename T, typename ...S>
    static std::string compose(T arg, S ...args) {
        return util::stringify(arg) + "//" + compose(args...);
    }

};

#ifndef NODEBUG
static API api;
#endif

namespace test {
    struct Assertion {
        bool result;
        std::string expected;
        std::string got;
    };
} // test

template <typename T>
static test::Assertion EQUAL(T returned, T expected) {
    return {util::equal(returned, expected), util::stringify(returned), util::stringify(expected)};
}

template <typename T>
static test::Assertion SMALLER(T returned, T expected) {
    return {util::smaller(returned, expected), util::stringify(returned), util::stringify(expected)};
}

template <typename T>
static test::Assertion GREATER(T returned, T expected) {
    return {util::greater(returned, expected), util::stringify(returned), util::stringify(expected)};
}

static void TEST(std::string description, test::Assertion a) {
#ifndef NODEBUG
    api.send(api.compose("TEST",
                         (a.result ? "PASS" : "FAIL"),
                         description,
                         a.expected,
                         a.got));
#endif
}

static void INFO(std::string s) {
#ifndef NODEBUG
    api.send(api.compose("LOG", "INFO", s));
#endif
}

static void WARN(std::string s) {
#ifndef NODEBUG
    api.send(api.compose("LOG", "WARN", s));
#endif
}

#endif // TUNA_H
