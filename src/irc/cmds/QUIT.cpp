#include "IRC.hpp"

void    IRC::QUIT( const Cmd & cmd, vector<t_ClientMsg> & res ) {
    //  Convert UserList type map to set
    set<User *> userSet;
    transform(_userList.cbegin(), _userList.cend(), inserter(userSet, userSet.begin()),
        [](const pair<string, User *>& value) { return value.second; });
    //  Send Quit msg to others on the server
    string  reason = !cmd._params[0].empty() ? cmd._params[0] : "";
    Emit2(cmd._user, userSet, getServMsg(cmd._user, MSG_QUIT, (string[]){reason}), res, true);
    //  Send ERROR msg to reply to the user
    PushToRes(cmd._user->getFd(), getServMsg(cmd._user, MSG_ERROR, (string[]){"QUIT requested"}), res);
    DeleteOffUser(cmd._user->getFd());
}
