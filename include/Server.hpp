#ifndef SERVER_HPP
# define SERVER_HPP

# include "Preset.hpp"
# include "Client.hpp"


# define MAX_LISTEN	42			

class Server {
	private:
		const int			_port;
		int					_fd;
		const string		_password;

		map<int, Client *>	_clientList;
		int					_lastFD;
		fd_set				_fdReader;

		void	AcceptClient();
		void	SetFDs();
		int		GetAllFDs();
		void	WaitCommand(int allFDs);
		int		ServerError(const string &msg);

	
	public:
		Server(int port, const string &password);
		virtual ~Server();

		void Init();
		void Watch();
};


#endif