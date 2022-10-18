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
    else if (_cmd == "NAMES") USER(res);
    else if (_cmd == "INVITE") INVITE(res);
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

//  ex)  NAMES #twilight_zone,#42 : takes one parameter
//  Split by "," must be done firstly.
//
//  Need to add a case : when users with the invisible user mode set are not shown in channel responses
//  unless the requesting client is also joined to that channel.
//
//  For now, only considered normal channel with "=" preset
void    Cmd::NAMES( vector<t_ClientMsg> & res )
{
    //  List all channel members
    if (_params.empty())
    {
        for (map<string, Channel *>::iterator it = _chanList.begin() ; 
            it != _chanList.end() ; ++it )
        {
            string nicks = it->second->getNicks();
            PushToRes(_user->_fd, getServReply(_user,  RPL_NAMREPLY, (string[]){ "=" + it->first, nicks }), res);
        }
    }
    else
    {
        vector<string>  givenNames = ::split(_params[0], ",");
        for (vector<string>::iterator it(givenNames.begin()) ; it != givenNames.end() ; ++it)
        {
            //  If the channel doesn't exist, send only RPL_ENDOFNAMES
            map<string, Channel *>::iterator chanIt;
            chanIt = _chanList.find(*it);

            if (chanIt != _chanList.end())
            {
                string nicks = chanIt->second->getNicks();
                PushToRes(_user->_fd, getServReply(_user,  RPL_NAMREPLY, (string[]){ "=" + *it, nicks }), res);
            }
            PushToRes(_user->_fd, getServReply(_user,  RPL_ENDOFNAMES, (string[]){ *it }), res);
        }
        //  if the given channel has the secret channel mode set
        //  and the user is not joined to that channel return RPL_ENDOFNAMES
    }
}

void    Cmd::INVITE( vector<t_ClientMsg> & res )
{
    if (_params.size() != 2)
        PushToRes(_user->_fd, getServReply(_user,  ERR_NEEDMOREPARAMS, (string[]){ _cmd }), res);
    else
    {
        map<string, Channel *>::iterator chanIt;
        chanIt = _chanList.find(_params[1]);
        //  ERR if the channel doesn't exist
        if (chanIt == _chanList.end())
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){ _params[1] }), res);
        else
        {
            set<User *> users = chanIt->second->_userList;
            User * invited = getUserByNick(_params[0]);
            if (invited == NULL) // Not defined by MAN..
                PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){ _params[0]}), res);
            else
            {
                set<User *>::iterator it = users.find(invited);
                set<User *>::iterator it2 = users.find(_user);
                //  ERR if the invited user is on the channel already
                if (it != users.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_USERONCHANNEL, (string[]){ _params[0], _params[1]}), res);
                //  ERR if the requesting user is not a member of the channer
                else if (it2 == users.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){ _params[1] }), res);
                else if (chanIt->second->_i)
                {
                    //  ERR if the channel has invite-only mode set, and the user is not a channel operator.
                    set<User *> opers = chanIt->second->_operList;
                    set<User *>::iterator   operIt;

                    operIt = opers.find(_user);
                    if (operIt == opers.end())
                        PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){ _params[1]}), res);
                }
                else
                {
                    //  If invite is successful
                    chanIt->second->addUser(invited);
                    invited->join(chanIt->second);
                    //  Reply to the requesting user
                    PushToRes(_user->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1]}), res);
                    //  Msg to the invited user
                    PushToRes(invited->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1], _user->getNick()}), res);
                }
            }
        }
    }
}

void    Cmd::PushToRes( int fd, const string &msg, vector<t_ClientMsg> &res ) {
    res.push_back(make_pair(fd, msg));
}