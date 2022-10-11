#ifndef USER_HPP
# define USER_HPP

# include "Preset.hpp"

class   User
{
    
    private:

        int             _fd;
        std::string     _nick;
        std::string     _uname;
        std::string     _rname;
        std::string     _mode;
        bool            _registered;

    public:

        User( int fd );
        virtual ~User();

};

#endif