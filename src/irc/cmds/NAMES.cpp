#include "IRC.hpp"

//  ex)  NAMES #twilight_zone,#42
//  For now, only considered normal(public) channel with "=" preset
void    IRC::NAMES( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    //  List all channel members
    if (cmd._params.empty())
    {
        for (map<string, Channel *>::iterator it = _chanList.begin() ; 
            it != _chanList.end() ; ++it )
        {
            Channel * chan = GetChannelByName(it->first);
            if (chan->_userList.find(cmd._user) != chan->_userList.end())
                PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= "+chan->getName(),chan->getNicks(false)}), res);
            else
                //  If the usr isn't on the channel,
                //  users with the invisible user mode set are not shown in channel responses
                PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= "+chan->getName(),chan->getNicks(true)}), res);
            PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){chan->getName()}), res);
        }
    }
    else
    {
        vector<string>  givenNames = ::split(cmd._params[0], ",");
        for (vector<string>::iterator it(givenNames.begin()) ; it != givenNames.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            //  If the channel doesn't exist, send only RPL_ENDOFNAMES
            if (chan != NULL)
            {
                if (chan->_userList.find(cmd._user) != chan->_userList.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= "+chan->getName(),chan->getNicks(false)}), res);
                else
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_NAMREPLY, (string[]){"= "+chan->getName(),chan->getNicks(true)}), res);
            }
            PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){chan->getName()}), res);
        }
    }
}