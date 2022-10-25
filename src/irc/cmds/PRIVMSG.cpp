#include "IRC.hpp"

void    IRC::PRIVMSG( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    if (_params.empty())
        PushToRes(_user->_fd, getServReply(_user, ERR_NORECIPIENT, NULL), res);
    else if (_params.size() < 2)
        PushToRes(_user->_fd, getServReply(_user, ERR_NOTEXTTOSEND, NULL), res);
    else
    {
        vector<string> targets = split(_params[0], ",");
        for (int i = 0; i < targets.size(); ++i)
        {
            //  Target is a chan
            if (targets[i].find(CHAN_PREFIX) != string::npos)
            {
                Channel * chan = GetChannelByName(targets[i]);
                //  ERR if the channel doesn't exist
                if (chan == NULL)
                    PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){targets[i]}), res);
                else
                {
                    //  when prefixed, msg will be delivered only to the membs of given status in the channel
                    if (_params[0][0] == OPER_PREFIX)
                        ;//IRC::Emit(_user, (string []){_params[1]}, chan->_operList, res, false);
                    else
                        ;//IRC::Emit(_user, (string []){_params[1]}, chan->_userList, res, false);
                }
            }
            // Target is a user
            else
            {
                User * target = getUserByNick(_params[0]);
                //  usr not on the server
                if (target == NULL)
                    PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){targets[i]}), res);
                else
                {
                    if (target->_away == "G")
                        PushToRes(_user->_fd, getServReply(_user,  RPL_AWAY, (string[]){targets[i]}), res);
                    PushToRes(target->_fd, getServReply(_user, MSG_PRIVMSG, (string[]){_params[1]}), res);
                }
            }
        }
    }
}