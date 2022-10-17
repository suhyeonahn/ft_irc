#include "Cmd.hpp"
Cmd::Cmd(
        string const & cmd,
        vector<string> params,
        User * user,
        map<int, User *> & userList,
        map<string, Channel *> & chanList
    ) : _cmd(cmd), _params(params), _user(user), _userList(userList), _chanList(chanList) { }

Cmd::~Cmd() {}

void    Cmd::execute(vector<t_ClientMsg> & res ) {
    if (_cmd == "PASS") PASS(res);
    else if (_cmd == "NICK") NICK(res);
    else if (_cmd == "USER") USER(res);
    else if (_cmd == "JOIN") JOIN(res);
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
        PushToRes(_user->_fd, servReply, res);
}

void    Cmd::NICK( vector<t_ClientMsg> & res )
{
    string  servReply;

    if (_params.empty())
        servReply = getServReply(_user, ERR_NONICKNAMEGIVEN, NULL); //431
    else
    {
        string const   &nick(_params[0]);

        if (!_user->isValidNick(nick)) 
            servReply = getServReply(_user, ERR_ERRONEUSNICKNAME, (string[]){ _cmd }); //432
        else if (getUserByNick(nick)) 
            servReply = getServReply(_user, ERR_NICKNAMEINUSE, (string[]){ _cmd }); //433
        else {
            _user->setNick(nick);
            //reply welcome msg to client
            Cmd::PushToRes(_user->_fd, getServReply(_user, RPL_WELCOME, NULL), res); // 001
        }
    }

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}

void    Cmd::USER( vector<t_ClientMsg> & res )
{
    string  servReply;

    // when client send again USER
    if (_user->_isRegistered)
        servReply = getServReply(_user, ERR_ALREADYREGISTERED, (string[]){ _cmd });
    // when not enough param
    else if (_params.size() < 4) 
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ _cmd });
    else {
        _user->setUname(_params[0]);
        _user->setRname(_params[3]);
        _user->_isRegistered = true;
    }

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}


//FIXME: put error case etc
// ERR_NEEDMOREPARAMS (461)
// ERR_NOSUCHCHANNEL (403)
// ERR_NOTONCHANNEL (442)
// ERR_CHANOPRIVSNEEDED (482)
// RPL_NOTOPIC (331)
// RPL_TOPIC (332)
// RPL_TOPICWHOTIME (333)
void Cmd::TOPIC( vector<t_ClientMsg> & res) {
    string  servReply;
    Channel *chan;

    //ERROR RPLY
    
    //No error    
    chan = _chanList[_params[0]];
    servReply = getServReply(_user, RPL_TOPIC, (string[]){ chan->_topic });

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}

// JOIN(client) - resp(server) -> MODE(client) -resp(server) -> WHO(client)...
void    Cmd::JOIN( vector<t_ClientMsg> & res ) {
    string  servReply;
    Channel *chan;

    //ERROR_RPLY
    if (_params.size() < 1) {
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ _cmd });
        PushToRes(_user->_fd, servReply, res);
        return ;
    }

    vector<string> names;
    vector<string> keys;
    names = split(_params[0], ",");
    if (_params.size() > 1)
        keys = split(_params[1], ",");
    
    for (int i = 0; i < names.size(); ++i) {
        const string &name = names[i];
        const string &key = (i < keys.size()) ? keys[i] : "";

        if (!Channel::IsValidName(name)) {
            servReply = getServReply(_user, ERR_BADCHANMASK, (string []) { name });
            PushToRes(_user->_fd, servReply, res);
            continue;
        }

        //TODO: modify here...
        // if (_chanList.find(name) == _chanList.end()) 
        //     _chanList[name] = (chan = new Channel(_params[0], _user));
        // else 
        //     chan = _chanList[name];
        // _user->_joined.insert(chan);

        // if (chan->_topic.size()) 
        //     TOPIC(res);
        // NAMES()...

    }
}

void    Cmd::PushToRes( int fd, const string &msg, vector<t_ClientMsg> &res ) {
    res.push_back(make_pair(fd, msg));
}