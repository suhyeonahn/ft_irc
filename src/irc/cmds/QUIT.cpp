#include "IRC.hpp"

void    IRC::QUIT( const Cmd & cmd, vector<t_ClientMsg> & res ) {
    string  msg = !cmd._params[0].empty() ? cmd._params[0] : "No reason";

    Emit(cmd._user, (string[]){"QUIT", ":" + msg, ""}, IRC::GetSameChanUsers(cmd._user), res, true);
    // DeleteOffUser(cmd._user->getFd());
}
