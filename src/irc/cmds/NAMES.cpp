#include "IRC.hpp"

//  ex)  NAMES #twilight_zone,#42
//  For now, only considered normal(public) channel with "=" preset
void    IRC::NAMES( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    Channel *chan;
    string  servReply;
    // string  name;
    bool    isJoined;
    //  List all channel members
    if (cmd._params.empty())
    {
        map<string, Channel *>::iterator it = _chanList.begin();
        // name = "*";
        for (it = _chanList.begin(); it != _chanList.end() ; ++it ) {
            chan = GetChannelByName(it->first);
            if (chan != NULL) {
                isJoined = chan->_userList.find(cmd._user) != chan->_userList.end();
                servReply += getServReply(cmd._user,  RPL_NAMREPLY, (string[]){" = " + chan->getName(), chan->getNicks(isJoined)});
            }
        }
    }
    else
    {
        vector<string>  givenNames = ::split(cmd._params[0], ",");
        cout << "HERE" << endl;
        for (vector<string>::iterator it(givenNames.begin()) ; it != givenNames.end() ; ++it)
        {
            chan = GetChannelByName(*it);
            if (chan != NULL) {
                cout << chan->_name << endl;
                isJoined = chan->_userList.find(cmd._user) != chan->_userList.end();
                servReply += getServReply(cmd._user,  RPL_NAMREPLY, (string[]){" = " + chan->getName(), chan->getNicks(isJoined)});
            }
        }
    }
    servReply += getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){cmd._params[0]});
    PushToRes(cmd._user->getFd(), servReply, res);
}

//   "<client> <channel> :End of /NAMES list"