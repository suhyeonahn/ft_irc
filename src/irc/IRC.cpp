//TODO: IRC class to process cmd sent by client and to response to irc client

#include "IRC.hpp"

IRC::IRC( const string &pw ) : _pw(pw) {}

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
	string   cmdStr = "";

	// New user registration
	if (_userList.find(fd) == _userList.end()) 
		user = (_userList[fd] = new User(fd, _pw));
	else
		user = _userList[fd];
	
	// Split msg to cmd(s)
	vector<string>  cmds = ::split(msg.second, SEP_MSG);

	initCmdList();

	// Execute cmd(s)
	for (std::vector<string>::iterator it(cmds.begin()) ; it != cmds.end() ; ++it)
	{
		cmdStr = setCmd(*it);

		// Check if the cmd exists
		// TODO: Send an err numeric accordingly 
		if (!isValid(cmdStr)) 
			Cmd::PushToRes(fd, getServReply(user, ERR_UNKNOWNCOMMAND, (string[]){cmdStr}), res);
		else if (isImplemented(cmdStr)) // Check if the cmd is implemented in our IRC
		{
			vector<string> params = setParams(*it);
			Cmd cmd(cmdStr, params, user, _userList, _chanList);
			cmd.execute(res);
			if (cmdStr == "QUIT")
				return true;
		}
	}
	return false;
}

void	IRC::DeleteOffUser(int fd, map<int, User *> &userList, map<string, Channel *> &chanList) {
	if (userList.find(fd) != userList.end()) {
		User *user = userList[fd];

		//Delete user in chanList
		map<string, Channel *>::iterator chanIt;
		Channel *chan;

		for (chanIt = chanList.begin(); chanIt != chanList.end(); ++chanIt) {
			chan = chanIt->second;
			if (chan->_invitedList.find(user) != chan->_invitedList.end()) {
				//remove joined channel in user instance
				if (user->_joined.find(chan) != user->_joined.end()) {
					user->_joined.erase(chan);
					chan->_userList.erase(user);
					chan->_operList.erase(user);
					// if no more user in chan, remove chan
					if (chan->_userList.size() == 0) {
						chanList.erase(chan->_name);
						delete chan;
					}
				} else if (chan->_invitedList.find(user) != chan->_invitedList.end())
					chan->_invitedList.erase(user);
			}
		}
		delete user;
		userList.erase(fd);
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

//FIXME: to debug
// userList size() is not correct after quick
// ==93936==ERROR: AddressSanitizer: heap-use-after-free on address 0x000103603978 at pc 0x0001004f0c38 bp 0x00016f951000 sp 0x00016f950ff8
string	IRC::Emit(User *user, string params[], set<User *> userList, vector<t_ClientMsg> &res, bool excludeUser) {
    string  msg = ":" + user->getNick();

    for (int i = 0; !params[i].empty(); ++i )
        msg += " " + params[i];
    msg += SEP_MSG;

    set<User *>::iterator it;
	cout << "HERE:" << userList.size() << endl;
    for(it = userList.begin(); it != userList.end(); ++it) {
        if (*it != user || !excludeUser)
            Cmd::PushToRes((*it)->getFd(), msg, res);
    }
    return msg;
}


void  IRC::test()
{
	bool plus;
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
		else
			; //  skip err
		for(string::size_type i = 1; i < tmp.size(); ++i)
			std::cout << tmp[i] << endl;
	}
}







void IRC::PRINT_USER_SET(set<User *> userset, const string &type) {
	cout << "\t - " << type << "(" << userset.size()<< ")" ;
	if (userset.size()) {
		set<User *>::iterator it;
		cout << ": [ ";
		for (it = userset.begin(); it != userset.end(); ++it) {
			// [ (fd1) nick1, (fd2) nick2, ...]
			cout << (it != userset.begin() ? ", " :"")
				 << "(" << (*it)->_fd << ") "
				 << (*it)->_nick;;
		}
		cout << " ]";
	}
	cout << endl;
}
void IRC::PRINT_STRING(const string &current, const string &type) {
	cout << "\t - " << type;
	if (!current.empty())
		cout << ": " << current;
	cout << endl;
}

void IRC::DEBUG() {
	cout << "\nIRC::DEBUG()" << endl;
	//userList
	cout << "- All User in IRC Server(" << _userList.size() << ")";
	if (_userList.size()) {
		map<int , User *>::iterator userIt;

		cout << ": [ ";
		for (userIt = _userList.begin(); userIt != _userList.end(); ++userIt) {
			//(fd) nick 
			cout << (userIt != _userList.begin() ? ", " :"")
				 << "(" << userIt->first << ") "
				 << userIt->second->_nick;
		}
		cout << " ]";
	}
	cout << endl;


	//chanlist
	if (_chanList.size()) {
		map<string , Channel *>::iterator chanIt;
		cout << "- Channel List: " << endl;
		for (chanIt = _chanList.begin(); chanIt != _chanList.end(); ++chanIt) {
			Channel *chan = chanIt->second;
			//Channel name
			cout << "\t[ " << chanIt->first << " ]" << endl;
			//topic
			PRINT_STRING(chan->_topic, "topic");
			//key
			PRINT_STRING(chan->_key, "key");

			PRINT_USER_SET(chan->_userList, "userList");
			PRINT_USER_SET(chan->_operList, "operList");
			PRINT_USER_SET(chan->_invitedList, "invitedList");
		}
	}
}