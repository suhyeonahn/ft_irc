#include "Channel.hpp"

Channel::Channel( string const & name, User * creator ) : _name(name), _i(false) {
    _userList.insert(creator);
    _operList.insert(creator);
}
Channel::~Channel() {

}


bool Channel::IsPrefix( char c ) {
    if (c == CHAN_PREFIX) return true;
    return false;
}
bool Channel::IsValidName(const string &name) {
    if (!IsPrefix(name[0]))  return false;

    static const string InvalidChars(CHAN_INVALID_CHAR);
    const set<char>     nameSet(name.begin() + 1, name.end());
    set<char>::const_iterator it;

    for (it = nameSet.begin(); it != nameSet.end() ; it++)
        if (InvalidChars.find(*it) != string::npos) return false;
    return true;
}

void    Channel::addUser( User * user )
{
    _userList.insert(user);
}

void    Channel::rmUser( User * user )
{
    _userList.erase(user);
}

void    Channel::sendMsg( int code, string params[], vector<t_ClientMsg> & res )
{
    for (set<User *>::iterator it(_userList.begin()) ; it != _userList.end() 
        ; ++it)
    {
        User * user = *it;
        Cmd::PushToRes(user->getFd(), getServReply(user, code, params), res);
    }
}

string  Channel::getName() const
{
    return _name;
}

string  Channel::getTopic() const
{
    return _topic;
}

string  Channel::getNicks() const
{
    string nicks = "";

    for (set<User *>::iterator it(_userList.begin()) ; it != _userList.end() 
        ; ++it)
    {
        User * user = *it;
        nicks += user->getNick();
        nicks += " ";
    }
    nicks.erase(nicks.end() - 1); // Remove last " " Char

    return _name;
}

size_t  Channel::getNusers() const
{
    size_t  N = 0;
    for (set<User *>::iterator it(_userList.begin()) ; it != _userList.end() 
        ; ++it)
        N++;
    return N;
}
