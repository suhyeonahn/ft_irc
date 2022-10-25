#include "IRC.hpp"

//  ex)  NAMES #twilight_zone,#42 : takes one parameter
//  Split by "," must be done firstly.
//
//  Need to add a case : when users with the invisible user mode set are not shown in channel responses
//  unless the requesting client is also joined to that channel.
//
//  For now, only considered normal channel with "=" preset
void    IRC::NAMES( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    //  List all channel members
    if (cmd._params.empty())
    {
        for (map<string, Channel *>::iterator it = _chanList.begin() ; 
            it != _chanList.end() ; ++it )
        {
            string nicks = it->second->getNicks();
            PushToRes(cmd._user->_fd, getServReply(cmd._user,  RPL_NAMREPLY, (string[]){ "= " + it->first, nicks }), res);
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
                string nicks = chan->getNicks();
                PushToRes(cmd._user->_fd, getServReply(cmd._user,  RPL_NAMREPLY, (string[]){ "= " + *it, nicks }), res);
            }
            PushToRes(cmd._user->_fd, getServReply(cmd._user,  RPL_ENDOFNAMES, (string[]){ *it }), res);
        }
    }
}