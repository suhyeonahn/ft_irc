#include "IRC.hpp"

void    IRC::QUIT( const Cmd &cmd, vector<t_ClientMsg> & res ) {
    string  msg = !_params[0].empty() ? _params[0] : "No reason";

    // IRC::Emit(_user, (string[]){"QUIT", ":" + msg, ""}, IRC::GetSameChanUsers(_user), res, true);
    IRC::DeleteOffUser(_user->_fd);
}
