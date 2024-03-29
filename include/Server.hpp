#ifndef SERVER_HPP
# define SERVER_HPP

# include "Preset.hpp"
# include "Client.hpp"
# include "IRC.hpp"

# define MAX_LISTEN	42			

class Server {
	private:
		const int			_port;
		int					_fd;
		const string		_password;
		IRC	&				_irc;	

		map<int, Client *>	_clientList;
		int					_lastFD;
		fd_set				_fdReader;

		void	AcceptClient();
		void 	DeleteClientFD(int fd);

		void	SetFDs();
		int		GetAllFDs();
		void	WaitClientMsg(int allFDs, vector<t_ClientMsg> &res, set<int> &offList);

		int		Error(const string &msg);

	public:
		Server(int port, const string &password, IRC & irc);
		virtual ~Server();

		void Init();
		void Watch();
		
		//function to debug current infos in server
		void DEBUG();
};


#endif