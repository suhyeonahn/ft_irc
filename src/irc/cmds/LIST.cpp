#include "IRC.hpp"

//  No need to send an ERR msg according to MAN
void    IRC::LIST( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (_params.empty())
    {
        //  Return the information about all visible channels (except hidden channels)
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTSTART, NULL), res);
        for (map<string, Channel *>::iterator it = _chanList.begin() ; 
            it != _chanList.end() ; ++it )
            PushToRes(_user->_fd, getServReply(_user,  RPL_LIST, (string[]){it->first,
                intToStr(it->second->getNusers()), it->second->getTopic()}), res);
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTEND, NULL), res);
    }
    else
    {
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTSTART, NULL), res);
        vector<string>  givenChans = ::split(cmd._params[0], ",");
        for (vector<string>::iterator it(givenChans.begin()) ; it != givenChans.end() ; ++it)
        {
            Channel * chan = GetChannelByName(*it);
            if (chan != NULL)
                PushToRes(_user->_fd, getServReply(_user,  RPL_LIST, (string[]){chan->getName(),
                    intToStr(chan->getNusers()), chan->getTopic()}), res);
        }
        PushToRes(_user->_fd, getServReply(_user,  RPL_LISTEND, NULL), res);
    }
}
