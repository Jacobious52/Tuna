#ifndef TEST_H
#define TEST_H

#include "api.hpp"

namespace test {
    struct Assertion {
        bool result;
        std::string expected;
        std::string got;
    };
} // test

template <typename T>
static test::Assertion EQUAL(T returned, T expected) {
    return {util::equal(returned, expected), util::stringify(expected), util::stringify(returned)};
}

template <typename T>
static test::Assertion SMALLER(T returned, T expected) {
    return {util::smaller(returned, expected), util::stringify(expected), util::stringify(returned)};
}

template <typename T>
static test::Assertion GREATER(T returned, T expected) {
    return {util::greater(returned, expected), util::stringify(expected), util::stringify(returned)};
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

#endif // TEST_H
