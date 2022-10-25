#include "IRC.hpp"

//  Send a rpy for each channel
void    IRC::PART( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    if (_params.size() < 1)
        PushToRes(_user->_fd, getServReply(_user,  ERR_NEEDMOREPARAMS, (string[]){ _cmd }), res);
    else
    {
        vector<string>  givenChans = ::split(_params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            //  ERR if the channel doesn't exist
            if (chan == NULL)
                PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){ _params[0] }), res);
            //  ERR if the user is not a member of the channer
            else if (_user->_joined.find(chan) == _user->_joined.end())
                PushToRes(_user->_fd, getServReply(_user, ERR_NOTONCHANNEL, (string[]){ _params[0] }), res);
            else
            {
                //  If PART is successful
                //  Inform the channel that the user has left
                // PushToRes(_user->_fd, getServReply(_user, MSG_PART, (string[]){ _user->getNick(), chan->_name }), res);
                // chan->sendMsg(MSG_PART, (string[]){_user->getNick(),chan->getName()}, res);

                //FIXME: message have to like :  ':dan-!d@localhost PART #test'
                // PushToRes(_user->_fd, ":" + _user->_nick + "!d@127.0.0.1 " + "PART " + chan->_name + SEP_MSG , res);
            }
        }
    }
}