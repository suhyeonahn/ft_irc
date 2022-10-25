#include "IRC.hpp"


//  The NOTICE message is used similarly to PRIVMSG.
//  The difference between NOTICE and PRIVMSG is that
//  automatic replies must never be sent in response to a NOTICE message. 
void    IRC::NOTICE( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() >= 2)
    {
        vector<string> targets = split(cmd._params[0], ",");
        for (int i = 0; i < targets.size(); ++i)
        {
            //  Target is a chan
            if (targets[i].find(CHAN_PREFIX) != string::npos)
            {
                Channel * chan = GetChannelByName(targets[i]);
                if (chan != NULL)
                {
                    //  when prefixed, msg will be delivered only to the membs of given status in the channel
                    if (cmd._params[0][0] == OPER_PREFIX)
                        ;//IRC::Emit(cmd._user, (string []){_params[1]}, chan->_operList, res, false);
                    else
                        ;//IRC::Emit(cmd._user, (string []){_params[1]}, chan->cmd._userList, res, false);
                }
            }
            // Target is a user
            else
            {
                User * target = getUserByNick(cmd._params[0]);
                if (target != NULL)
                    PushToRes(target->_fd, getServReply(cmd._user, MSG_PRIVMSG, (string[]){cmd._params[1]}), res);
            }
        }
    }
}