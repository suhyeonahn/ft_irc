#include "User.hpp"

User::User( int fd, string const servPw ) : _fd(fd), _servPw(servPw), _nick(DEFAULT_NAME), _uname(DEFAULT_NAME),
    _rname(DEFAULT_NAME), _isGoodPw(false), _isRegistered(false),
    _i(false), _o(false), _O(false), _r(false), _w(true) {}

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

void    User::setMode( bool plus, char const & mode )
{
    if (mode == 'i')
        _i = plus ? true : false;
    if (mode == 'o')
        _o = plus ? true : false;
    if (mode == 'O')
        _O = plus ? true : false;
    if (mode == 'r')
        _r = plus ? true : false;
    if (mode == 'w')
        _w = plus ? true : false;
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

bool    User::isValidMode( char const & mode )
{
    static string const     validChars(USER_MODE);

    if (validChars.find(mode) == string::npos)
        return false;
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

string    User::getMode()  const
{
    string mode = "";
    if (_i)
        mode += "i";
    if (_o)
        mode += "o";
    if (_O )
        mode += "O";
    if (_r)
       mode += "r";
    if (_w)
        mode += "w";
    return mode;   
}

int     User::getFd() const
{
    return  _fd;
}
