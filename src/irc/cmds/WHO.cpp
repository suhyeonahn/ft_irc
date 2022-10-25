#include "IRC.hpp"

//  Didn't consider mask pattern case
//"<client> <channel> <username> <host> <server> <nick> <flags> :<hopcount> <realname>"
//ex:
// WHO emersion        ; request information on user "emersion"
//:calcium.libera.chat 352 dan #ircv3 ~emersion sourcehut/staff/emersion calcium.libera.chat emersion H :1 Simon Ser
//:calcium.libera.chat 315 dan emersion :End of WHO list

void    IRC::WHO( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    if (_params.empty()) {
        PushToRes(_user->_fd, getServReply(_user, ERR_NEEDMOREPARAMS, (string[]){_cmd}), res);
        return ;
    } else if (Channel::IsPrefix(_params[0][0])) {
		// case channel name => send all user in channel whose name corresponding to params[0]
		Channel * chan = GetChannelByName(_params[0]);

		if (chan) {
			set<User *>::iterator it;
			for (it = chan->_userList.begin(); it != chan->_userList.end(); ++it) {
				PushToRes(
					_user->_fd,
					getServReply(*it, RPL_WHOREPLY, (string[]){ (*it)->getWho(_params[0])}),
					res
				);
			}
		}
    } else {
		// case exact nickname => send user info whose nickname correspond to params[0]
		map<int, User *>::iterator it;
		User *current;

		for (it = _userList.begin(); it != _userList.end(); ++it) {
			current = it->second;
			if (current->_nick == _params[0]) // if found exact nickname correspond to params[0]
				PushToRes(
					_user->_fd,
					getServReply(current, RPL_WHOREPLY, (string[]){ current->getWho("*") }),
					res
				);
		}
    }
    PushToRes(_user->_fd, getServReply(_user, RPL_ENDOFWHO, NULL), res);
}