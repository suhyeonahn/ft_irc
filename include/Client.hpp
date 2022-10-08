#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Preset.hpp"

# define BUF_SIZE 512

class Client {
    private:
        const int   _fd;
        char        _buf[BUF_SIZE + 1]; // to add '\0' at the last of buffer


        Client();

        bool        RecvCommand(string &cmd);

    public:
        Client(int fd);
        ~Client();

        friend class Server;
};
#endif