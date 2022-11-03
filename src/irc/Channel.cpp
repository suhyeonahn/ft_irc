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

bool    Channel::isValidMode( string const & mode )
{
    static string const     validChars(CHAN_MODE);
    for (unsigned long i = 0 ; i < mode.size(); i++)
    {
        if (validChars.find(mode[i]) == string::npos)
            return false;
    }
    return true;
}

void    Channel::setMode( bool plus, string const & mode )
{
    for (unsigned long i = 0 ; i < mode.size(); i++)
    {
        if (mode[i] == 'i')
            _i = plus ? true : false;
        if (mode[i] == 'k')
            _k = plus ? true : false;
    }
}

void    Channel::addUser( User * user )
{
    _userList.insert(user);
}

void    Channel::rmUser( User * user )
{
    _userList.erase(user);
    _operList.erase(user);
}

string  Channel::getName() const
{
    return _name;
}

string  Channel::getTopic() const
{
    return _topic;
}

string  Channel::getNicks( bool i ) const
{
    string nicks = "";

    for (set<User *>::iterator it(_userList.begin()) ; it != _userList.end() 
        ; ++it)
    {
        User * user = *it;
        //  Users with the invisible user mode set are not shown in channel responses
        //  unless the requesting client is also joined to that channel.
        if (!(i == false && user->_i == true))
        {
            if (getOperByNick(user->getNick()))
                nicks += OPER_PREFIX;
            nicks += user->getNick() + " ";
        }
    }
    if (!nicks.empty())
        nicks.erase(nicks.end() - 1); // Remove last " " Char

    return nicks;
}

size_t  Channel::getNusers() const
{
    size_t  N = 0;
    for (set<User *>::iterator it(_userList.begin()) ; it != _userList.end() 
        ; ++it)
        N++;
    return N;

}

string    Channel::getMode()  const
{
    string mode = "";
    if (_i)
       mode += "i";
    if (_k)
        mode += "k";
    return mode;
}

User *  Channel::getOperByNick( string const & nick ) const
{
	for (set<User *>::const_iterator it(_operList.begin());
		it != _operList.end(); ++it)
    {
        User * user = *it;
		if (user->getNick() == nick)
			return user;
    }
	return NULL;
}