#include "Cmd.hpp"

Cmd::Cmd( string const & cmd, vector<string> params, User * user)
    : _cmd(cmd), _params(params), _user(user)
{}

Cmd::~Cmd() {}

void    Cmd::PASS()
{
    if (_params.empty())
        ; // err numeric
    if (_params[0] == _user->_servPw)
        _user->_isGoodPw = true;
}

void    Cmd::NICK()
{
    std::string const   &nick(_params[0]);
    if (nick == _user->getNick())
        return  ;
    else
        _user->setNick(nick);
}

void    Cmd::USER()
{
    if (_params.size() < 4)
        ; // err numeric
    else
    {
        _user->setUname(_params[0]);
        _user->setRname(_params[3]);
        _user->_isRegistered = true;
    }
}