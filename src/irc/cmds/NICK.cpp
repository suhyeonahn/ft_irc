#include "IRC.hpp"

void    IRC::NICK( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    string  servReply;

    if (cmd._params.empty())
        servReply = getServReply(_user, ERR_NONICKNAMEGIVEN, NULL); //431
    else
    {
        string const   &nick(cmd._params[0]);

        if (!_user->isValidNick(nick)) 
            servReply = getServReply(_user, ERR_ERRONEUSNICKNAME, (string[]){ cmd._cmd }); //432
        else if (getUserByNick(nick)) 
            servReply = getServReply(_user, ERR_NICKNAMEINUSE, (string[]){ cmd._cmd }); //433
        else {
            _user->setNick(nick);
            //reply welcome msg to client
            PushToRes(_user->_fd, getServReply(_user, RPL_WELCOME, NULL), res); // 001
        }
    }

    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}