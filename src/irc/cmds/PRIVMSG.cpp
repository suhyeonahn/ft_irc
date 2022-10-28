#include "IRC.hpp"

void    IRC::PRIVMSG( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NORECIPIENT, NULL), res);
    else if (cmd._params.size() < 2)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOTEXTTOSEND, NULL), res);
    else
    {
        vector<string> targets = split(cmd._params[0], ",");
        for (size_t i = 0; i < targets.size(); ++i)
        {
            //  Target is a chan
            if (targets[i].find(CHAN_PREFIX) != string::npos)
            {
                Channel * chan = GetChannelByName(targets[i]);
                //  ERR if the channel doesn't exist
                if (chan == NULL)
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){targets[i]}), res);
                else
                {
                    //  when prefixed, msg will be delivered only to the membs of given status in the channel
                    if (cmd._params[0][0] == OPER_PREFIX)
                        Emit2(cmd._user, chan->_operList, getServMsg(cmd._user, MSG_PRIVMSG, (string[]){chan->getName(),cmd._params[1]}), res, false);
                    else
                        Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_PRIVMSG, (string[]){chan->getName(),cmd._params[1]}), res, false);
                }
            }
            // Target is a user
            else
            {
                User * target = getUserByNick(cmd._params[0]);
                //  usr not on the server
                if (target == NULL)
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOSUCHNICK, (string[]){targets[i]}), res);
                else
                {
                    if (target->_away == "G")
                        PushToRes(cmd._user->getFd(), getServReply(cmd._user,  RPL_AWAY, (string[]){target->getNick()}), res);
                    PushToRes(target->getFd(), getServMsg(cmd._user, MSG_PRIVMSG, (string[]){target->getNick(),cmd._params[1]}), res);
                }
            }
        }
    }
}