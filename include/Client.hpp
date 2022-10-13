#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Preset.hpp"

# define BUF_SIZE 512
# define SEP_MSG "\r\n"
# define SEP_LEN 2

class Client {
    private:
        const int   _fd;
        char        _buf[BUF_SIZE + 1]; // to add '\0' at the last of buffer
        string      _msg;


        Client();

        bool        RecvMsg(string & msg);
        void        SendRes(const string &res);

    public:
        Client(int fd);
        ~Client();

        friend class Server;
};
#endif