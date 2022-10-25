#include "IRC.hpp"

void    IRC::NICK( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NONICKNAMEGIVEN, NULL), res);
    else
    {
        string const & nick(cmd._params[0]);

        if (!cmd._user->isValidNick(nick)) 
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_ERRONEUSNICKNAME, (string[]){nick}), res);        else if (getUserByNick(nick)) 
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NICKNAMEINUSE, (string[]){nick}), res);
        else {
            cmd._user->setNick(nick);
            //reply welcome msg to client
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_WELCOME, NULL), res);
        }
    }
}