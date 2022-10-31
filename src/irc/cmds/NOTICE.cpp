#include "IRC.hpp"


//  The NOTICE message is used similarly to PRIVMSG.
//  The difference between NOTICE and PRIVMSG is that
//  automatic replies must never be sent in response to a NOTICE message. 
void    IRC::NOTICE( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() >= 2)
    {
        vector<string> targets = split(cmd._params[0], ",");
        for (size_t i = 0; i < targets.size(); ++i)
        {
            //  Target is a chan
            if (targets[i].find(CHAN_PREFIX) != string::npos)
            {
                Channel * chan;
                if (targets[i][0] == OPER_PREFIX)
                    chan = GetChannelByName(&targets[i][1]);
                else
                    chan = GetChannelByName(targets[i]);
                if (chan != NULL)
                {
                    //  when prefixed, msg will be delivered only to the membs of given status in the channel
                    if (targets[i][0] == OPER_PREFIX)
                        Emit2(cmd._user, chan->_operList, getServMsg(cmd._user, MSG_NOTICE, (string[]){chan->getName(),cmd._params[1]}), res, false);
                    else
                        Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_NOTICE, (string[]){chan->getName(),cmd._params[1]}), res, false);
                }
            }
            // Target is a user
            else
            {
                User * target = getUserByNick(cmd._params[0]);
                if (target != NULL)
                    PushToRes(target->getFd(), getServMsg(cmd._user, MSG_NOTICE, (string[]){target->getNick(),cmd._params[1]}), res);
            }
        }
    }
}