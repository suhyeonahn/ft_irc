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
	vector<t_ClientMsg> res;
	vector<t_ClientMsg>::iterator resIt;

	while (1) {
		res.clear();

		SetFDs();
		WaitClientMsg(GetAllFDs(), res);

		//loop to send response to each clientFD
		for(resIt = res.begin(); resIt!= res.end(); ++resIt) {
			int clientFD = resIt->first;
			if (_clientList.find(clientFD) != _clientList.end()) {
				cout << resIt->first << ":" << resIt->second << endl;
				_clientList[clientFD]->SendRes(resIt->second);
			}
		}
		DEBUG();
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

void Server::DeleteClient(int fd) {
	if (_clientList.find(fd) != _clientList.end()) {
		delete _clientList[fd];
		_clientList.erase(fd);
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
void	Server::WaitClientMsg(int allFDs, vector<t_ClientMsg> &res) {
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
				if (!_clientList[fd]->RecvMsg(msg)) {
					//Although select() said client socket is changed,
					//if lengh of buffer sent by client is , client socket is disconnected.
					//So, delete client in _clientList

					//TODO: delete client from channel
					//like DeleteClientFromChannel(fd)
					//(this code will be implemented after finish JOIN functionality)
					DeleteClient(fd);
				}
				else if (!msg.empty()) {
					_irc.ProcessClientMsg(make_pair(fd, msg), res);

					//LINE TO DEBUG
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