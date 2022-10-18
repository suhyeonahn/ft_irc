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
        //FIXED: nick.find -> validChars.find(IT_OF_NICK_SET)
        //Before, if nick doesn't contain all valid charset, return false.
        //ex: nick_kyu1 => false : because it doesn't contain ALL valid charset
        //But, idea was so good(we don't need see all char in string but each char not repeating):)
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
