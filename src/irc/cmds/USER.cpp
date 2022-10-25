#include "IRC.hpp"

void    IRC::USER( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    // when client send again USER
    if (_user->_isRegistered)
        servReply = getServReply(_user, ERR_ALREADYREGISTERED, (string[]){ cmd._cmd });
    // when not enough param
    else if (cmd._params.size() < 4) 
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ cmd._cmd });
    else {
        _user->setUname(cmd._params[0]);
        _user->setRname(cmd._params[3]);
        _user->_isRegistered = true;
    }

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}

