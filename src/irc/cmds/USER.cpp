#include "IRC.hpp"

void    IRC::USER( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    // when client send again USER
    if (cmd._user->_isRegistered)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_ALREADYREGISTERED, NULL), res);
    // when not enough param
    else if (cmd._params.size() < 4) 
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    else {
        cmd._user->setUname(cmd._params[0]);
        cmd._user->setRname(cmd._params[3]);
        cmd._user->_isRegistered = true;
    }
}