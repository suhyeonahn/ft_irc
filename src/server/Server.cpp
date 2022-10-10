#include "Server.hpp"

Server::Server(int port, const string &password, IRC & irc) : _port(port), _password(password), _irc(irc) {

}
Server::~Server(){

}

void	Server::Init(){
	protoent *proto(getprotobyname("tcp"));

#if defined (__APPLE__)
	_fd = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (_fd != -1)
		fcntl(_fd, F_SETFL, O_NONBLOCK);
#else
	_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, proto->p_proto);
#endif
	if (_fd == -1)
		Error("socket");

	int opt = 1;
	/**
	  * make socket reutilisable
	  * ex:
	    when server is stopped, socket is not yet close directly.
	  * this function make to use which has been just used, managing socket level.
	  */
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		Error("setsock");

	sockaddr_in			servSIN;
	servSIN.sin_family = AF_INET;
	servSIN.sin_addr.s_addr = htonl(INADDR_ANY);
	servSIN.sin_port = htons(_port);

	if (bind(_fd, (sockaddr *)&servSIN, sizeof(servSIN)) == -1)
		Error("bind");
	if (listen(_fd, MAX_LISTEN) == -1)
		Error("listen");
	cout << CYN << "FT_IRC Server up to be inited to " << _port << DFT << endl;
}


void	Server::Watch() {
	while (1) {
		SetFDs();
		WaitClientMsg(GetAllFDs());
	}
}

void Server::AcceptClient() {
	sockaddr_in	clientSIN;
	socklen_t	lenSIN = 0;

	int clientFD = accept(_fd, (sockaddr *)&clientSIN, &lenSIN);

	if (clientFD == -1) 
		cerr << "Failed to accept" << endl;
	else {
		cout << "New client on socket #" << clientFD << endl;
		_clientList.insert(make_pair(clientFD, new Client(clientFD)));
	}
}

void	Server::SetFDs() {
	_lastFD = _fd;
	FD_ZERO(&_fdReader);
	FD_SET(_fd, &_fdReader);

	// if new client has been added
	std::map<int, Client *>::iterator clientIt;
	for (clientIt = _clientList.begin(); clientIt != _clientList.end(); ++clientIt) {
		int clientFD = clientIt->first;
		FD_SET(clientFD, &_fdReader);
		if (clientFD > _lastFD)
			_lastFD = clientFD;
	}
}

/**
 * select() verify if there is change in fd_set struct
 * Here, we need only file reader in socket because we will not write(or modify) fd
 * returns the number of ready descriptors that are contained in the descriptor sets
*/
int		Server::GetAllFDs() {
	int ret = select(_lastFD + 1, &_fdReader, NULL, NULL, NULL);
	if (ret == -1)
		Error("select");
	return ret;
}

/**
 * Wait command by processor 
 * if client connected => AcceptClient()
 * else if client send cmd => _clientList[current_client]->RecvCommand(cmd)
 * else if client disconnected => TO MANAGE THIS
 */
void	Server::WaitClientMsg(int allFDs) {
	string	msg;

	for (int fd = 3; fd <= _lastFD && allFDs; ++fd) {
		// verify if current fd is set by _fdReader
		if (FD_ISSET(fd, &_fdReader)) {
			//if serverFD
			if (fd == _fd)
				AcceptClient();
			else {
				// if clientFD
				msg.clear();
				_clientList[fd]->RecvMsg(msg);
				if (!msg.empty()) {
					//TODO: create IRC class to process cmd sent by client and to response to irc client
					// In this moment test with this command(or with hexchat) to get cmd saved in this cmd variable.
					// TERMINAL 1: ./ircserv <PORT>
					// TERMINAL 2: echo -ne 'GET / HTTP/1.1\r\nHost: example.com\r\n\r\n' | nc 127.0.0.1 <PORT>
					// It will be send like :

					// ❯ ./ircserv 6666 1234
					// 	FT_IRC Server up to be inited to 6666
					// 	New client on socket #4
					// 	client #4:
					// 	GET / HTTP/1.1
					// 	Host: example.com
					_irc.ProcessClientMsg(std::make_pair(fd, msg));

					cout << "client #" << fd << ":" << endl;
					cout << CYN << msg << DFT;
				}
			}
			--allFDs;
		}
	}
}


int		Server::Error(const string &msg) {
	cerr << RED << msg << " : " << strerror(errno) << DFT << endl;
	exit(1);
}

