#include "IRC.hpp"
#include "Server.hpp"

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

void	Server::DEBUG() {

	cout << MAG << "[DEBUG]" << endl;
	cout << "- Port: " << _port << endl;
	cout << "- Password: " << _password << endl;
	cout << "- Server FD: " << _fd << endl;
	cout << "- Client FDs: ";
	if (_clientList.size() > 0) {
		map<int, Client *>::iterator it;
		cout << "[ ";
		for(it = _clientList.begin(); it != _clientList.end(); ++it) {
			cout << (it != _clientList.begin() ? ", " : "") << it->first;
		}
		cout << " ]" << endl;
	}
	_irc.DEBUG();
	cout << DFT << endl;
}