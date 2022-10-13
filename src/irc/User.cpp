#include "User.hpp"

User::User( int fd, string const servPw ) : _fd(fd), _servPw(servPw), _nick(DEFAULT_NAME), _uname(DEFAULT_NAME),
    _rname(DEFAULT_NAME), _isGoodPw(false), _isRegistered(false) {}

User::~User() {}

void    User::setNick( std::string const & nick )
{
    _nick = nick;
}

void    User::setUname( std::string const & uname )
{
    _uname = uname;
}

void    User::setRname( std::string const & rname )
{
    _rname = rname;
}

std::string    User::getNick() const
{
    return  _nick;
}

std::string    User::getUname() const
{
    return  _uname;
}

void    User::execute()
{
    switch ( _cmd )
    {
        case 0:  { PASS(); }
        case 1:  { NICK(); }
        case 2:  { USER(); }
        default:    ;
    }

}