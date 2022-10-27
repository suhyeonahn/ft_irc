#include "IRC.hpp"

void    IRC::INVITE( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() < 2)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    else
    {
        Channel * chan = GetChannelByName(cmd._params[1]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){ cmd._params[1] }), res);
        else
        {
            User * invited = getUserByNick(cmd._params[0]);
            //  ERR if the invited user doesn't exist on server
            //  Not defined by MAN..
            if (invited == NULL)
                PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOSUCHNICK, (string[]){cmd._params[0]}), res);
            else
            {
                //  ERR if the invited user is on the channel already
                if (invited->_joined.find(chan) != cmd._user->_joined.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_USERONCHANNEL, (string[]){cmd._params[0],cmd._params[1]}), res);
                //  ERR if the requesting user is not a member of the channer
                else if (cmd._user->_joined.find(chan) == cmd._user->_joined.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){cmd._params[1]}), res);
                //  ERR if the channel has invite-only mode set, and the user is not a channel operator.
                else if (chan->_i && chan->_operList.find(cmd._user) == chan->_operList.end())
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_CHANOPRIVSNEEDED, (string[]){cmd._params[1]}), res);
                else
                {
                    //  If invite is successful
                    chan->addUser(invited);
                    invited->join(chan);
                    //  Reply to the requesting user
                    PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_INVITING, (string[]){invited->getNick(),chan->getName()}), res);
                    //  Msg to the invited user
                    PushToRes(invited->getFd(), getServMsg(cmd._user, MSG_INVITE, (string[]){invited->getNick(),chan->getName()}), res);
                }
            }
        }
    }
}