#ifndef API_H
#define API_H

#include "util.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

#endif // API_H
