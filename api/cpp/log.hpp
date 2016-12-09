#ifndef LOG_H
#define LOG_H

#include "api.hpp"

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

#endif // LOG_H
