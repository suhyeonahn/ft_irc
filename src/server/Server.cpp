#include "Server.hpp"

Server::Server(int port, const string &password) : _port(port), _password(password) {

}
Server::~Server(){

}
void	Server::init(){
	protoent *proto(getprotobyname("tcp"));

#if defined (__APPLE__)
	_fd = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (_fd != -1)
		fcntl(_fd, F_SETFL, O_NONBLOCK);
#else
	_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, proto->p_proto)
#endif
	if (_fd == -1)
		serv_error("socket");

	int opt = 1;
	/**
	  * make socket reutilisable
	  * ex:
	    when server is stopped, socket is not yet close directly.
	  * this function make to use which has been just used, managing socket level.
	  */
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		serv_error("setsock");

	sockaddr_in			serv_sin;
	serv_sin.sin_family = AF_INET;
	serv_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_sin.sin_port = htons(_port);

	if (bind(_fd, (sockaddr *)&serv_sin, sizeof(serv_sin)) == -1)
		serv_error("bind");
	if (listen(_fd, MAX_LISTEN) == -1)
		serv_error("listen");
	cout << CYN << "FT_IRC Server up to be inited to " << _port << DFT << endl;
}

void Server::acceptClient() {
	sockaddr_in	client_sin;
	socklen_t	sin_len = 0;

	int clientFd = accept(_fd, (sockaddr *)&client_sin, &sin_len);

	if (clientFd == -1) 
		cerr << "Failed to accept" << endl;
	else {
		cout << "New client on socket #" << clientFd << endl;
		_clientList.insert(make_pair(clientFd, new Client(clientFd)));
	}
}

void	Server::watch() {
	while (1) {
		_maxFd = _fd;
		FD_ZERO(&_fdReader);
		FD_SET(_fd, &_fdReader);

		std::map<int, Client *>::iterator clientIter;
		for (clientIter = _clientList.begin(); clientIter != _clientList.end(); ++clientIter) {
			int clientFd = clientIter->first;
			if (clientFd > _maxFd)
				_maxFd = clientFd;
		}
		// select() verify if there is change in fd_set struct
		// Here, we need only file reader in socket because we will not write(or modify) fd
		int r = select(_maxFd + 1, &_fdReader, NULL, NULL, NULL);
		if (r != -1)
			acceptClient();
	}
}

int		Server::serv_error(const string &msg) {
		cerr << RED << msg << " : " << strerror(errno) << DFT << endl;
		exit(1);
}

