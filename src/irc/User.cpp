#include "User.hpp"

User::User( int fd, string const servPw ) : _fd(fd), _servPw(servPw), _nick(DEFAULT_NAME), _uname(""),
    _rname(""), _isGoodPw(false), _isRegistered(false),
     _away("H"), _i(false), _o(false) {}

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

void    User::setMode( bool plus, string const & mode )
{
    for (unsigned long i = 0 ; i < mode.size(); i++)
    {
        if (mode[i] == 'i')
           _i = plus ? true : false;
    }
}

bool    User::isValidNick( string const & nick )
{
    static string const     validChars(VALID_CHARS_NICK);
    set<char> const  nickSet(nick.begin(), nick.end());

    for (set<char>::const_iterator it(nickSet.begin()) ; it != nickSet.end() ; it++)
    {
        if (validChars.find(*it) == string::npos) 
            return false;
    }
    return true;
}

bool    User::isValidMode( string const & mode )
{
    static string const     validChars(USER_MODE);

for (unsigned long i = 0 ; i < mode.size(); i++)
    {
        if (validChars.find(mode[i]) == string::npos)
            return false;
    }
    return true;
}

void    User::join( Channel * chan )
{
    _joined.insert(chan);
}

string  User::getNick()  const
{
    return  _nick;
}

string  User::getUname() const
{
    return  _uname;
}

string  User::getMode()  const
{
    string mode = "";
    if (_i)
        mode += "i";
    if (_o)
        mode += "o";
    return mode;   
}

string  User::getWho(const string chan) const {
    //  <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>
    string res = chan;

    res += (" " + _uname + " " + USR_HOST + " " + SERV_HOST + " " + _nick + " " + _away + " :1 " +_rname);
    return res;
}


int     User::getFd() const
{
    return  _fd;
}




// if (res.size() > 1)
//     res = res.substr(1);
// i put hopcount '1' with hard-coding because there is only one server in this project.

// string  User::getWho() const
// {
//     //  <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>
//     string who = "";
//     who += getJoinedChan() + " ";
//     who += _uname + " ";
//     //  TODO: what does host signify
//     who += "suky ";  //  server
//     who += _nick + " ";
//     who += _away + " ";
//     who += getMode() + " ";
//     who += ":1 ";   //  hopcount is always 1 cuz we have only 1 server
//     who += _rname;
//     return who;
// }

// string  User::getJoinedChan() const
// {
//     string  chans  = "";
//     for (set<Channel *>::iterator it(_joined.begin()) ; it != _joined.end() ; ++it)
//     {
//         Channel * chan = *it;
//         chans += chan->getName() + " ";
//     }
//     if (chans == "") // No joined chans
//         chans = "*";
//     else
//         chans.erase(chans.end() - 1); // Remove last " " Char
//     return chans;
// }