#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {
    // Set fd non-blocking
    fcntl(_fd, F_SETFL, O_NONBLOCK);
}

Client::~Client() {

}

bool    Client::RecvCommand(string &cmd) {
    ssize_t len = recv(_fd, _buf, BUF_SIZE, 0);
    if (len <= 0)
        return false;
    _buf[len] = '\0';
    cmd = string(_buf);
    return true;
}
