#include "IRC.hpp"

void    IRC::OPER( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.size() < 2)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    else if (!(cmd._params[0] == OPER_NAME && cmd._params[1] == OPER_PASS))
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_PASSWDMISMATCH, NULL), res);
    else
    {
        cmd._user->_o = true;
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_YOUREOPER, NULL), res);
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_UMODEIS, (string[]){cmd._user->getMode()}), res);
    }
}