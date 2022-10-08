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

    _cmd += _buf;
    
    // To see ascii number of each character.
    // for (string::iterator it = _cmd.begin(); it != _cmd.end() ; ++it) {
    //     cout << (int)*it << endl;
    // }

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
