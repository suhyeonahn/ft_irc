#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Preset.hpp"
// # include "Server.hpp"

class Client {
    private:
        int _fd;

        Client();

    public:
        Client(int fd);
        ~Client();
};
#endif