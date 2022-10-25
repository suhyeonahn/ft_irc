#include "IRC.hpp"

void    IRC::USER( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    // when client send again USER
    if (_user->_isRegistered)
        servReply = getServReply(_user, ERR_ALREADYREGISTERED, (string[]){ _cmd });
    // when not enough param
    else if (_params.size() < 4) 
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ _cmd });
    else {
        _user->setUname(_params[0]);
        _user->setRname(_params[3]);
        _user->_isRegistered = true;
    }

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}

