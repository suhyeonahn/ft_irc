#include "IRC.hpp"

void    IRC::USER( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    // when client send again USER
    if (cmd._user->_isRegistered)
        servReply = getServReply(cmd._user, ERR_ALREADYREGISTERED, (string[]){ cmd._cmd });
    // when not enough param
    else if (cmd._params.size() < 4) 
        servReply = getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){ cmd._cmd });
    else {
        cmd._user->setUname(cmd._params[0]);
        cmd._user->setRname(cmd._params[3]);
        cmd._user->_isRegistered = true;
    }

    if (!servReply.empty())
        PushToRes(cmd._user->_fd, servReply, res);
}