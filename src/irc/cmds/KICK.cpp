#include "IRC.hpp"

//  Can kick a user at once
void    IRC::KICK( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() < 1)
        PushToRes(cmd._user->_fd, getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
     else
    {
        Channel * chan = GetChannelByName(cmd._params[0]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(cmd._user->_fd, getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){cmd._params[0]}), res);
        else
        {
            User * kicked = getUserByNick(cmd._params[1]);
            //  ERR if user is not on the channel
            if (kicked->_joined.find(chan) == kicked->_joined.end())
                PushToRes(cmd._user->_fd, getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){cmd._params[0],cmd._params[1]}), res);
            //  ERR if the channel has invite-only mode set, and the requesting user is not a channel operator.
            else if (chan->_i && chan->_operList.find(cmd._user) == chan->_operList.end())
                PushToRes(cmd._user->_fd, getServReply(cmd._user, ERR_CHANOPRIVSNEEDED, (string[]){cmd._params[0]}), res);
            else
            {
                //  If KICK is successful
                chan->rmUser(kicked);
                //  Inform the channel that the user has left
                chan->sendMsg(MSG_PART, (string[]){kicked->getNick(),chan->getName()}, res);
                //  Send a msg to the kicked user
                if (cmd._params.size() == 1)
                    PushToRes(kicked->_fd, getServReply(cmd._user, MSG_KICK, (string[]){cmd._params[0]}), res);
                else
                    PushToRes(kicked->_fd, getServReply(cmd._user, MSG_KICK, (string[]){cmd._params[0],cmd._params[1]}), res);
            }
        }
    }

}
