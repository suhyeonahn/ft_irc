//TODO: IRC class to process cmd sent by client and to response to irc client

#include "IRC.hpp"

IRC::IRC( const string &pw ) : _pw(pw) {
	Cmd::initCmdList();
}

IRC::~IRC()
{
	for (map<int, User *>::iterator it = _userList.begin() ; it != _userList.end() ; ++it )
		delete it->second;

	for (map<string, Channel *>::iterator it = _chanList.begin() ; 
		it != _chanList.end() ; ++it )
		delete it->second;
}

/**
 *  return true if client send QUIT else false;
 */
bool   IRC::ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res)
{
	User *   user;
	int      fd(msg.first);
	// New user registration
	if (_userList.find(fd) == _userList.end()) {
		cout << "HERE" << endl;
		user = (_userList[fd] = new User(fd, _pw));
		if (_pw.empty())
			user->_isGoodPw = true;
	} else {
		cout << "HERE2" << endl;
		cout << _userList.size() << endl;

		user = _userList[fd];
	}
	
	// Split msg to cmd(s)
	vector<string>  cmds = ::split(msg.second, SEP_MSG);

		cout << user << endl;


	// Execute cmd(s)
	for (vector<string>::iterator it(cmds.begin()) ; it != cmds.end() ; ++it)
	{
		Cmd cmd(user, *it);
		bool registered(user->_isRegistered);


		// Check if the cmd exists
		// TODO: Send an err numeric accordingly
		if (!cmd.isValid()) 
			PushToRes(fd, getServReply(user, ERR_UNKNOWNCOMMAND, (string[]){cmd._cmd}), res);
		else if (cmd.isImplemented()) // Check if the cmd is implemented in our IRC
		{
			execute(cmd, res);
			if (cmd._cmd == "QUIT") {
				return true;
			}
			if (!cmd._user->_isGoodPw) {
				PushToRes(user->_fd, "You are not authentificated!\r\n", res);
				DeleteOffUser(user->_fd);
				return true;
			}
			// first registered 
			if (!registered && user->_isRegistered) {
	            PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_WELCOME, NULL), res);
				MOTD(cmd, res);
			}
		}
	}
	return false;
}

void	IRC::DeleteOffUser(int fd) {
	if (_userList.find(fd) != _userList.end()) {
		User *user(_userList[fd]);
		//Delete user in chanList

		if (user) {
			map<string, Channel *> copy(_chanList);
			map<string, Channel *>::iterator chanIt;
			Channel *chan;

			if (user->_isRegistered) {
				for (chanIt = copy.begin(); chanIt != copy.end(); ++chanIt) {
					chan = chanIt->second;

					//remove joined channel in user instance
					if (user->_joined.find(chan) != user->_joined.end()) {
						user->_joined.erase(chan);
						chan->_userList.erase(user);
						chan->_operList.erase(user);

						// // if no more user in chan, remove chan
						if (chan->_userList.size() == 0) {
							_chanList.erase(chan->_name);
							delete chan;
						}

					} else if (chan->_invitedList.find(user) != chan->_invitedList.end())
						chan->_invitedList.erase(user);
				}
			}
			delete user;
			_userList.erase(fd);
		}
	}
}

// returns all users in same channel with current user passed by param
set<User *> IRC::GetSameChanUsers(User *user){
	set<User *> 				res;
	set<Channel *>::iterator	chanIt;

	for(chanIt = user->_joined.begin(); chanIt != user->_joined.end(); ++chanIt)
		res.insert((*chanIt)->_userList.begin(), (*chanIt)->_userList.end());
	res.erase(user);
	return res;
}

//I moved this function to IRC from Channel to use it on divers cases.
// send message to userList
// if excludeUser is true, except current user
// else current user will be also receive msg
// returns msg which could be useful depending to case.
// string	IRC::Emit(User *user, string params[], const set<User *> &userList, vector<t_ClientMsg> &res, bool excludeUser) 
string	IRC::Emit(User *user, string params[], const set<User *> &userList, vector<t_ClientMsg> &res, bool excludeUser) {
    string  msg = ":" + user->getNick();

    for (int i = 0; !params[i].empty(); ++i )
        msg += " " + params[i];
    msg += SEP_MSG;

    set<User *>::iterator it;
    for(it = userList.begin(); it != userList.end(); ++it) {
        if (*it != user || !excludeUser)
            PushToRes((*it)->getFd(), msg, res);
    }
    return msg;
}

void	IRC::Emit2( User * user, const set<User *> & userList, string msg, vector<t_ClientMsg> & res, bool excludeUser )
{
    set<User *>::iterator it;
    for(it = userList.begin(); it != userList.end(); ++it)
	{
		if (!(*it == user && excludeUser == true))
        	PushToRes((*it)->getFd(), msg, res);
	}
}


// Execs

void    IRC::execute(const Cmd &cmd, vector<t_ClientMsg> & res ) {
    if (cmd._cmd == "PASS") PASS(cmd, res);
    else if (cmd._cmd == "NICK") NICK(cmd, res);
    else if (cmd._cmd == "USER") USER(cmd, res);
	else if (!cmd._user->_isRegistered) return ;
    else if (cmd._cmd == "JOIN") JOIN(cmd, res);
    else if (cmd._cmd == "TOPIC") TOPIC(cmd, res);
    else if (cmd._cmd == "NAMES") NAMES(cmd, res);
    else if (cmd._cmd == "INVITE") INVITE(cmd, res);
    else if (cmd._cmd == "PART") PART(cmd, res);
    else if (cmd._cmd == "LIST") LIST(cmd, res);
    else if (cmd._cmd == "KICK") KICK(cmd, res);
    else if (cmd._cmd == "WHO") WHO(cmd, res);
    else if (cmd._cmd == "QUIT") QUIT(cmd, res);
    else if (cmd._cmd == "PRIVMSG") PRIVMSG(cmd, res);
    else if (cmd._cmd == "NOTICE") NOTICE(cmd, res);
	else if (cmd._cmd == "PING") PONG(cmd, res);
	else if (cmd._cmd == "MOTD") MOTD(cmd, res);
	else if (cmd._cmd == "MODE") MODE(cmd, res);
	else if (cmd._cmd == "KILL") KILL(cmd, res);
	else if (cmd._cmd == "OPER") OPER(cmd, res);
}

User *  IRC::getUserByNick( string const & nick ) const
{
	for (map<int, User *>::const_iterator it(_userList.begin());
		it != _userList.end(); ++it)
		if (it->second->_nick == nick)
			return it->second;
	return NULL;
}

Channel *IRC::GetChannelByName ( const string &name ) const {
    if (_chanList.find(name) != _chanList.end())
        return _chanList.at(name);
    return NULL;
}

Channel *IRC::CreateChannel( const string &name, User *user) {
    Channel *chan = new Channel(name, user);
    _chanList[name] = chan;
	// user->_O = true; // channel operator
    user->join(chan);
    return chan;
}


void    IRC::PushToRes( int fd, const string &msg, vector<t_ClientMsg> &res ) {
    res.push_back(make_pair(fd, msg));
}




void  IRC::test()
{
	bool plus;

	(void)plus;
	//  parse modestring
	string params = "i0+23l--on++=o-";
	vector<string> modeStr = splitModeStr(params, "+-");
	for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
	{
		string tmp = *it;
		if (tmp[0] == '+')
			plus = true;
		else if (tmp[1] == '-')
			plus = false;

		for(string::size_type i = 1; i < tmp.size(); ++i)
			std::cout << tmp[i] << endl;
	}
}



