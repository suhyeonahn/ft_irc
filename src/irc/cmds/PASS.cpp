#include "IRC.hpp"

void    IRC::PASS( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    if (cmd._params.empty())
        servReply = getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){ cmd._cmd }); //461
    else if (cmd._user->_isGoodPw)
        servReply = getServReply(cmd._user, ERR_ALREADYREGISTERED, NULL); //462
    else if (cmd._params[0] == cmd._user->_servPw)
        cmd._user->_isGoodPw = true;
    else
        servReply = getServReply(cmd._user, ERR_PASSWDMISMATCH, NULL); //464
    
    if (!servReply.empty())
        PushToRes(cmd._user->_fd, servReply, res);
}
