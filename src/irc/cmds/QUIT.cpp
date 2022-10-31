#include "IRC.hpp"

void    IRC::QUIT( const Cmd & cmd, vector<t_ClientMsg> & res ) {
    User *user(cmd._user);
    string  msg = !cmd._params.empty() ? "Quit: " + cmd._params[0] : "No reason";

    Emit(user, (string[]){"QUIT", ":" + msg, ""}, IRC::GetSameChanUsers(user), res, true);
    DeleteOffUser(user->_fd);
}
