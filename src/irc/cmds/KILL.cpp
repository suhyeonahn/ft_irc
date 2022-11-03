#include "IRC.hpp"

//  Can kick a user at once
void    IRC::KILL( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
     else
    {
        User * killed = getUserByNick(cmd._params[0]);
        //  ERR if and the requesting user is not a channel operator.
        if (!cmd._user->_o)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOPRIVILEGES, NULL), res);
        else if (killed != NULL && killed != cmd._user)
        {
            //  If KILL is successful
            string  reason = cmd._params.size() > 1 ? cmd._params[1] : "For some reason";
            PushToRes(killed->getFd(), getServMsg(cmd._user, MSG_KILL, (string[]){killed->getNick(),reason}), res);
            Emit2(killed, GetSameChanUsers(killed), getServMsg(killed, MSG_QUIT, (string[]){"Killed (" + cmd._user->getNick() + " (" + reason + ")"}), res, false);
            PushToRes(killed->getFd(), getServMsg(killed, MSG_ERROR, (string[]){"Closing Link: irc-suky (Killed ("+ cmd._user->getNick()+ " (" + reason + ")))"}), res);
            DeleteOffUser(killed->getFd());
        }
    }
}