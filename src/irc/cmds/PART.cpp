#include "IRC.hpp"

//  Send a rpy for each channel
void    IRC::PART( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() < 1)
        PushToRes(cmd._user->_fd, getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd }), res);
    else
    {
        vector<string>  givenChans = ::split(cmd._params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            //  ERR if the channel doesn't exist
            if (chan == NULL)
                PushToRes(cmd._user->_fd, getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){cmd._params[0] }), res);
            //  ERR if the user is not a member of the channer
            else if (cmd._user->_joined.find(chan) == cmd._user->_joined.end())
                PushToRes(cmd._user->_fd, getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){cmd._params[0] }), res);
            else
            {
                //  If PART is successful
                //  Inform the channel that the user has left
                // PushToRes(cmd._user->_fd, getServReply(cmd._user, MSG_PART, (string[]){ cmd._user->getNick(), chan->_name }), res);
                // chan->sendMsg(MSG_PART, (string[]){cmd._user->getNick(),chan->getName()}, res);

                //FIXME: message have to like :  ':dan-!d@localhost PART #test'
                // PushToRes(cmd._user->_fd, ":" + cmd._user->_nick + "!d@127.0.0.1 " + "PART " + chan->_name + SEP_MSG , res);
            }
        }
    }
}