#include "IRC.hpp"

//  Can kick a user at once
void    IRC::KICK( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
     else
    {
        Channel * chan = GetChannelByName(cmd._params[0]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){cmd._params[0]}), res);
        else
        {
            vector<string> targets = split(cmd._params[1], ",");
            for (size_t i = 0; i < targets.size(); ++i)
            {
                User * kicked = getUserByNick(targets[i]);
                //  ERR if user is not on the channel
                if (chan->_userList.find(cmd._user) == chan->_userList.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){chan->getName()}), res);
                else if (kicked == NULL || (kicked->_joined.find(chan) == kicked->_joined.end()))
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_USERNOTINCHANNEL, (string[]){chan->getName(),targets[i]}), res);
                //  ERR if and the requesting user is not a channel operator.
                else if (chan->_operList.find(cmd._user) == chan->_operList.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_CHANOPRIVSNEEDED, (string[]){chan->getName()}), res);
                else if (kicked == cmd._user)
                    ;
                else
                {
                    //  If KICK is successful
                    //  Inform the channel that the user has left
                    if (cmd._params.size() >= 3)
                        Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_KICK, (string[]){chan->_name,kicked->getNick(),cmd._params[2]}), res, false);
                    else
                        Emit2(cmd._user, chan->_userList, getServMsg(cmd._user, MSG_KICK, (string[]){chan->_name,kicked->getNick(),"For some reason"}), res, false);
                    kicked->_joined.erase(chan);
                    chan->rmUser(kicked);
                }
            }
        }
    }
}
