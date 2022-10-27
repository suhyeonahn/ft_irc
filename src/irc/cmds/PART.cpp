#include "IRC.hpp"
//  TODO: add function to remove the user from channel
//  Send a rpy for each channel
void    IRC::PART( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() < 1)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    else
    {
        vector<string>  givenChans = ::split(cmd._params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            //  ERR if the channel doesn't exist
            if (chan == NULL)
                PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){cmd._params[0]}), res);
            //  ERR if the user is not a member of the channer
            else if (cmd._user->_joined.find(chan) == cmd._user->_joined.end())
                PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){chan->getName()}), res);
            else
            {
                //  If PART is successful
                if (cmd._params.size() >= 2)
                    Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_PART, (string[]){chan->getName(),cmd._params[1]}), res, true);
                else
                    Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_PART, (string[]){chan->getName()}), res, true);
            }
        }
    }
}