#include "IRC.hpp"

void    IRC::PASS( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    if (cmd._params.empty())
        servReply = getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){ cmd._cmd }); //461
    else if (_user->_isGoodPw)
        servReply = getServReply(_user, ERR_ALREADYREGISTERED, NULL); //462
    else if (cmd._params[0] == _user->_servPw)
        _user->_isGoodPw = true;
    else
        servReply = getServReply(_user, ERR_PASSWDMISMATCH, NULL); //464
    
    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}
