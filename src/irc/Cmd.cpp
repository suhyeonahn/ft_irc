#include "Cmd.hpp"

Cmd::Cmd( string const & cmd, vector<string> params, User * user)
    : _cmd(cmd), _params(params), _user(user) {}

Cmd::~Cmd() {}

void    Cmd::execute(vector<t_ClientMsg> &res) {
}

void    Cmd::PASS(vector<t_ClientMsg> &res)
{
    if (_params.empty())
        ; // err numeric
    if (_params[0] == _user->_servPw)
        _user->_isGoodPw = true;
}

void    Cmd::NICK(vector<t_ClientMsg> &res)
{
    std::string const   &nick(_params[0]);
    if (nick == _user->getNick())
        return  ;
    else
        _user->setNick(nick);
}

void    Cmd::USER(vector<t_ClientMsg> &res)
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

void    Cmd::PushToRes(const string msg, vector<t_ClientMsg> &res) {
    res.push_back(make_pair(_user->_fd, msg));
}