#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {
    // Set fd non-blocking
    fcntl(_fd, F_SETFL, O_NONBLOCK);
}

Client::~Client() {
    close(_fd);
    cout << "Client on socket #" << _fd << " has been disconnected" << endl;
}

bool    Client::RecvCommand(string &cmd) {
    ssize_t len = recv(_fd, _buf, BUF_SIZE, 0);
    if (len <= 0)
        return false;
    _buf[len] = '\0';

    _cmd += _buf;
    /**
     * if cmd doesn't contains seperator("\r\n"), wait until cmd contains that.
     * else update cmd then clear _cmd in this instance
     */
    if (_cmd.find(SEP_CMD, _cmd.size() - SEP_LEN) != string::npos) {
        cmd = _cmd;
        _cmd.clear();
    }
    return true;
}
