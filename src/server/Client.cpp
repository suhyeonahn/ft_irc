#include "Client.hpp"

Client::Client(int fd) : _fd(fd) {
    // Set fd non-blocking
    fcntl(_fd, F_SETFL, O_NONBLOCK);
}

Client::~Client() {
    close(_fd);
    cout << "Client on socket #" << _fd << " has been disconnected" << endl;
}

bool    Client::RecvMsg(string &msg) {
    ssize_t len = recv(_fd, _buf, BUF_SIZE, 0);
    //len : -1 if error, 0 if fd is disconnected
    if (len <= 0) 
        return false;
    _buf[len] = '\0';

    _msg += _buf;
    /**
     * if msg doesn't finish by seperator("\r\n"), wait until msg contains that.
     * else update msg then clear _msg in this instance
     */
    if (_msg.find(SEP_MSG, _msg.size() - SEP_LEN) != string::npos) {
        msg = _msg;
        _msg.clear();
    }
    return true;
}

void    Client::SendRes(const string &res) {
    //TO DEBUG
    cout << BLU << res << DFT;
    send(_fd, res.c_str(), res.size(), 0);
}