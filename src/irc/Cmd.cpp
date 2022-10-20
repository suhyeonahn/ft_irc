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
    else if (_cmd == "NAMES") NAMES(res);
    else if (_cmd == "INVITE") INVITE(res);
    else if (_cmd == "PART") PART(res);
    else if (_cmd == "LIST") LIST(res);
    else if (_cmd == "KICK") KICK(res);
}

User *  Cmd::getUserByNick( string const & nick ) const
{
	for (map<int, User *>::const_iterator it(_userList.begin());
		it != _userList.end(); ++it)
		if (it->second->_nick == nick)
			return it->second;
	return NULL;
}

Channel *Cmd::GetChannelByName ( const string &name ) const {
    if (_chanList.find(name) != _chanList.end())
        return _chanList.at(name);
    return NULL;
}

Channel *Cmd::CreateChannel( const string &name, User *user) {
    Channel *chan = new Channel(name, user);
    _chanList[name] = chan;
    user->join(chan);
    return chan;
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
        chan = GetChannelByName(name);
        if (chan == NULL)
            chan = CreateChannel(name, _user);
        else {
            //if user not yet joined
            if (_user->_joined.find(chan) == _user->_joined.end()) {
                // if invite mode and user not invited
                if (chan->_i && chan->_invitedList.find(_user) == chan->_invitedList.end())
                    servReply = getServReply(_user, ERR_INVITEONLYCHAN, (string []) { name });
                // if channal has key and invalid key
                else if (!chan->_key.empty() && chan->_key != key)
                    servReply = getServReply(_user, ERR_BADCHANNELKEY, (string[]) { name });
                else {
                    //if all ok
                    //add user 
                    chan->addUser(_user);
                    //remove user in invite list
                    chan->_invitedList.erase(_user);
                    //add channel to user
                    _user->_joined.insert(chan);
                }
            }
        }
        if (!servReply.empty())
            PushToRes(_user->_fd, servReply, res);
        else {
            //1.send to all user in channel(NOT YET)
            PushToRes(_user->_fd, ":" + _user->_nick + " JOIN " + name + SEP_MSG, res);
            
            //2.topic(NOT YET)
            if (chan->_topic.size()) 
                TOPIC(res);
            //3. send names
            NAMES(res);
        }
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
            Channel * chan = GetChannelByName(*it);
            //  If the channel doesn't exist, send only RPL_ENDOFNAMES
            if (chan != NULL)
            {
                string nicks = chan->getNicks();
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
        Channel * chan = GetChannelByName(_params[1]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){ _params[1] }), res);
        else
        {
            User * invited = getUserByNick(_params[0]);
            //  ERR if the invited user doesn't exist on server
            //  Not defined by MAN..
            if (invited == NULL)
                PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){ _params[0]}), res);
            else
            {
                //  ERR if the invited user is on the channel already
                if (invited->_joined.find(chan) != _user->_joined.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_USERONCHANNEL, (string[]){ _params[0], _params[1]}), res);
                //  ERR if the requesting user is not a member of the channer
                else if (_user->_joined.find(chan) == _user->_joined.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){ _params[1] }), res);
                //  ERR if the channel has invite-only mode set, and the user is not a channel operator.
                else if (chan->_i && chan->_operList.find(_user) == chan->_operList.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){ _params[1]}), res);
                else
                {
                    //  If invite is successful
                    chan->addUser(invited);
                    invited->join(chan);
                    //  Reply to the requesting user
                    PushToRes(_user->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1]}), res);
                    //  Msg to the invited user
                    PushToRes(invited->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1], _user->getNick()}), res);
                }
            }
        }
    }
}

//  Send a rpy for each channel
void    Cmd::PART( vector<t_ClientMsg> & res )
{
    if (_params.size() < 1)
        PushToRes(_user->_fd, getServReply(_user,  ERR_NEEDMOREPARAMS, (string[]){ _cmd }), res);
    else
    {
        vector<string>  givenChans = ::split(_params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            //  ERR if the channel doesn't exist
            if (chan == NULL)
                PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){ _params[0] }), res);
            //  ERR if the user is not a member of the channer
            else if (_user->_joined.find(chan) == _user->_joined.end())
                PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){ _params[0] }), res);
            else
            {
                //  If PART is successful
                //  Inform the channel that the user has left
                // PushToRes(_user->_fd, getServReply(_user, MSG_PART, (string[]){ _user->getNick(), chan->_name }), res);
                // chan->sendMsg(MSG_PART, (string[]){_user->getNick(),chan->getName()}, res);

                //FIXME: message have to like :  ':dan-!d@localhost PART #test'
                // PushToRes(_user->_fd, ":" + _user->_nick + "!d@127.0.0.1 " + "PART " + chan->_name + SEP_MSG , res);
            }
        }
    }
}

//  No need to send an ERR msg according to MAN
void    Cmd::LIST( vector<t_ClientMsg> & res )
{
    if (_params.empty())
    {
        //  Return the information about all visible channels (except hidden channels)
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTSTART, (string[]){NULL}), res);
        for (map<string, Channel *>::iterator it = _chanList.begin() ; 
            it != _chanList.end() ; ++it )
            PushToRes(_user->_fd, getServReply(_user,  RPL_LIST, (string[]){it->first,
                intToStr(it->second->getNusers()), it->second->getTopic()}), res);
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTEND, (string[]){NULL}), res);
    }
    else
    {
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTSTART, (string[]){NULL}), res);
        vector<string>  givenChans = ::split(_params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            if (chan != NULL)
                PushToRes(_user->_fd, getServReply(_user,  RPL_LIST, (string[]){chan->getName(),
                    intToStr(chan->getNusers()), chan->getTopic()}), res);
        }
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTEND, (string[]){NULL}), res);
    }
}

//  Can kick a user at once
void    Cmd::KICK( vector<t_ClientMsg> & res )
{
    if (_params.size() < 1)
        PushToRes(_user->_fd, getServReply(_user,  ERR_NEEDMOREPARAMS, (string[]){_cmd}), res);
     else
    {
        Channel * chan = GetChannelByName(_params[0]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){_params[0]}), res);
        else
        {
            User * kicked = getUserByNick(_params[1]);
            //  ERR if user is not on the channel
            if (kicked->_joined.find(chan) == kicked->_joined.end())
                PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){_params[0],_params[1]}), res);
            //  ERR if the channel has invite-only mode set, and the requesting user is not a channel operator.
            else if (chan->_i && chan->_operList.find(_user) == chan->_operList.end())
                PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){ _params[0]}), res);
            else
            {
                //  If KICK is successful
                chan->rmUser(kicked);
                //  Inform the channel that the user has left
                chan->sendMsg(MSG_PART, (string[]){kicked->getNick(),chan->getName()}, res);
                //  Send a msg to the kicked user
                if (_params.size() == 1)
                    PushToRes(kicked->_fd, getServReply(_user, MSG_KICK, (string[]){_params[0]}), res);
                else
                    PushToRes(kicked->_fd, getServReply(_user, MSG_KICK, (string[]){_params[0],_params[1]}), res);
            }
        }
    }

}

void    Cmd::MODE( vector<t_ClientMsg> & res )
{
    //  Check if _params[0] is a nick or a chan
    if (_params[0][0] == CHAN_PREFIX)
    {
        //  Target is a chan
        Channel * chan = GetChannelByName(_params[0]);
        //  chan not on the server
        if (chan == NULL)
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){_params[0]}), res);
        else if (_params.size() < 2)
            PushToRes(_user->_fd, getServReply(_user, RPL_CHANNELMODEIS, (string[]){chan->getName(), chan->getMode()}), res);
        else
        {
            //  ERR if the user is not a channel operator.
            if (chan->_operList.find(_user) == chan->_operList.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){chan->getName()}), res);
        }
    }
    else
    {
        //  Target is a usr
        User * usr = getUserByNick(_params[0]);
        //  usr not on the server
        if (usr == NULL)
            PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){_params[0]}), res);
        //  _user is not matching the nick
        else if (_user != usr)
            PushToRes(_user->_fd, getServReply(_user, ERR_USERSDONTMATCH, (string[]){NULL}), res);
        //  modestring is not given then send a msg containing the current modes
        else if (_params.size() < 2)
            PushToRes(_user->_fd, getServReply(_user, RPL_UMODEIS, (string[]){_user->getMode()}), res);
        else
        {
            //  modestring is given
            bool isValid = true;
            bool plus;
            string  servReply;
            //  parse modestring
            vector<string> modeStr = splitModeStr(_params[1], "+-");
            for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
            {
                string token = *it;
                if (token[0] != '+' && token[0] != '-')
                    servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                else
                {
                    if (token[0] == '+')
                        plus = true;
                    else if (token[1] == '-')
                        plus = false;
                    for(string::size_type i = 1; i < token.size(); ++i)
                    {
                        if (isValid == _user->isValidMode(token[i]))
                            _user->setMode(plus, token[i]);
                        if (!isValid)
                            servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                    }
                }
            }
            //  Send changed mode
            PushToRes(_user->_fd, getServReply(_user, RPL_UMODEIS, (string[]){_user->getMode()}), res);
            if (!servReply.empty())
                PushToRes(_user->_fd, servReply, res);
        }
    }    
}

void    Cmd::PushToRes( int fd, const string &msg, vector<t_ClientMsg> &res ) {
    res.push_back(make_pair(fd, msg));
}
