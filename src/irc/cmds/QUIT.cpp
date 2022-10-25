#include "IRC.hpp"

void    IRC::QUIT( const Cmd & cmd, vector<t_ClientMsg> & res ) {
    string  msg = !cmd._params[0].empty() ? cmd._params[0] : "No reason";

    // IRC::Emit(cmd._user, (string[]){"QUIT", ":" + msg, ""}, IRC::GetSameChanUsers(cmd._user), res, true);
    IRC::DeleteOffUser(cmd._user->_fd);
}
