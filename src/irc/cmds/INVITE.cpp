#include "IRC.hpp"

void    IRC::INVITE( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    if (_params.size() != 2)
        PushToRes(_user->_fd, getServReply(_user,  ERR_NEEDMOREPARAMS, (string[]){ _cmd }), res);
    else
    {
        Channel * chan = GetChannelByName(_params[1]);
        //  ERR if the channel doesn't exist
        if (chan == NULL)
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){ _params[1] }), res);
        else
        {
            User * invited = getUserByNick(_params[0]);
            //  ERR if the invited user doesn't exist on server
            //  Not defined by MAN..
            if (invited == NULL)
                PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){ _params[0]}), res);
            else
            {
                //  ERR if the invited user is on the channel already
                if (invited->_joined.find(chan) != _user->_joined.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_USERONCHANNEL, (string[]){ _params[0], _params[1]}), res);
                //  ERR if the requesting user is not a member of the channer
                else if (_user->_joined.find(chan) == _user->_joined.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){ _params[1] }), res);
                //  ERR if the channel has invite-only mode set, and the user is not a channel operator.
                else if (chan->_i && chan->_operList.find(_user) == chan->_operList.end())
                    PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){ _params[1]}), res);
                else
                {
                    //  If invite is successful
                    chan->addUser(invited);
                    invited->join(chan);
                    //  Reply to the requesting user
                    PushToRes(_user->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1]}), res);
                    //  Msg to the invited user
                    PushToRes(invited->_fd, getServReply(_user, RPL_INVITING, (string[]){ _params[0], _params[1], _user->getNick()}), res);
                }
            }
        }
    }
}