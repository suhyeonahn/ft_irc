#include "User.hpp"

User::User( int fd, string const servPw ) : _fd(fd), _servPw(servPw), _nick(DEFAULT_NAME), _uname(DEFAULT_NAME),
    _rname(DEFAULT_NAME), _isGoodPw(false), _isRegistered(false), _servOper(false) { }

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

bool    User::isValidNick( string const & nick )
{
    static string const     validChars(VALID_CHARS_NICK);
    static set<char> const  nickSet(nick.begin(), nick.end());

    for (set<char>::const_iterator it(nickSet.begin()) ; it != nickSet.end() ; it++)
    {
        if (validChars.find(*it) == string::npos) 
            return false;
    }
    return true;
}

void    User::join( Channel * chan )
{
    _joined.insert(chan);
}

string    User::getNick()  const
{
    return  _nick;
}

string    User::getUname() const
{
    return  _uname;
}

int     User::getFd() const
{
    return  _fd;
}
