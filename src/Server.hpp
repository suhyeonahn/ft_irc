#ifndef SERVER_HPP
# define SERVER_HPP

# include "Preset.hpp"
# include "Client.hpp"

# define MAX_LISTEN	42			

class Client;

class Server {
	private:
		const int			_port;
		int					_fd;
		const string		_password;

		map<int, Client *>	_clientList;
		int					_maxFd;
		fd_set				_fdReader;

		void acceptClient();
		int	serv_error(const string &msg);

	
	public:
		Server(int port, const string &password);
		virtual ~Server();

		void init();
		void watch();
};


#endif