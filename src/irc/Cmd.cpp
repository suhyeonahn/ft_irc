#include "Cmd.hpp"

Cmd::Cmd( string const & cmd, vector<string> params, User * user, map<int, User *> & userList )
    : _cmd(cmd), _params(params), _user(user), _userList(userList) {}

Cmd::~Cmd() {}

void    Cmd::execute(vector<t_ClientMsg> & res ) {
    if (_cmd == "PASS") PASS(res);
    else if (_cmd == "NICK") NICK(res);
}

User *  Cmd::getUserByNick( string const & nick ) const
{
	for (map<int, User *>::const_iterator it(_userList.begin());
		it != _userList.end(); ++it)
		if (it->second->_nick == nick)
			return it->second;
	return NULL;
}

void    Cmd::PASS( vector<t_ClientMsg> & res )
{
    string  servReply;

    if (_params.empty())
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ _cmd }); //461
    else if (_user->_isGoodPw)
        servReply = getServReply(_user, ERR_ALREADYREGISTERED, NULL); //462
    else if (_params[0] == _user->_servPw)
        _user->_isGoodPw = true;
    else
        servReply = getServReply(_user, ERR_PASSWDMISMATCH, NULL); //464
    
    if (!servReply.empty())
        PushToRes(servReply, res);
}

void    Cmd::NICK( vector<t_ClientMsg> & res )
{
    string  servReply;

    if (_params.empty())
        servReply = getServReply(_user, ERR_NONICKNAMEGIVEN, NULL); //431
    else
    {
        string const   &nick(_params[0]);

        if (!_user->isValidNick(nick)) {
            std::cout << "HERE1\n";
            servReply = getServReply(_user, ERR_ERRONEUSNICKNAME, (string[]){ _cmd }); //432
        }
        else if (getUserByNick(nick)) {
            std::cout << "HERE2\n";
            servReply = getServReply(_user, ERR_NICKNAMEINUSE, (string[]){ _cmd }); //433
        }
        else {
            // servReply = getServReply(_user, -1, (string[]){ _cmd });
            _user->setNick(nick);
        }
    }

    if (!servReply.empty())
        PushToRes(servReply, res);
}

void    Cmd::USER( vector<t_ClientMsg> & res )
{
    string  servReply;

    if (_params.size() < 4)
        ; // err numeric
    else
    {
        _user->setUname(_params[0]);
        _user->setRname(_params[3]);
        _user->_isRegistered = true;
    }

    if (!servReply.empty())
        PushToRes(servReply, res);
}

void    Cmd::PushToRes( const string &msg, vector<t_ClientMsg> &res ) {
    res.push_back(make_pair(_user->_fd, msg));
}