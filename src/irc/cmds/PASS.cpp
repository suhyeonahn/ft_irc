#include "IRC.hpp"

void    IRC::PASS( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    else if (cmd._user->_isGoodPw)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_ALREADYREGISTERED, NULL), res);
    else if (cmd._params[0] == cmd._user->_servPw)
        cmd._user->_isGoodPw = true;
    else
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_PASSWDMISMATCH, NULL), res);
}
