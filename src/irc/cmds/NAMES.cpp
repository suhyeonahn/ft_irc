#include "IRC.hpp"

//  ex)  NAMES #twilight_zone,#42
//  For now, only considered normal(public) channel with "=" preset
void    IRC::NAMES( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    Channel *chan;
    string  servReply;
	string	name;
    bool    isJoined;
    if (cmd._params.empty())
    {
        map<string, Channel *>::iterator it = _chanList.begin();
		name = "*";
        for (it = _chanList.begin(); it != _chanList.end() ; ++it ) {
            chan = GetChannelByName(it->first);
            if (chan != NULL) {
                isJoined = chan->_userList.find(cmd._user) != chan->_userList.end();
                servReply += getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= " + chan->getName(), chan->getNicks(isJoined)});
            }
        }
        // servReply += getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){"*"});
    }
    else
    {
        vector<string>  givenNames = ::split(cmd._params[0], ",");
		name = cmd._params[0];
        for (vector<string>::iterator it(givenNames.begin()) ; it != givenNames.end() ; ++it)
        {
            chan = GetChannelByName(*it);
            if (chan != NULL) {
                isJoined = chan->_userList.find(cmd._user) != chan->_userList.end();
                servReply += getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= " + chan->getName(), chan->getNicks(isJoined)});
            }
        }
        // servReply += getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){chan->getName()});
    }
	servReply += getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){name});
    PushToRes(cmd._user->getFd(), servReply, res);
}
