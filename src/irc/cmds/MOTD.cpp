#include "IRC.hpp"

string	IRC::ReadMOTD() {
    ifstream	ifs("motd");
	string		data;
	stringstream ss;
	int			i = 4;

	if (!ifs) return data;

	while (getline(ifs, data)) {
		//  + 2 digit number(16-98 or 0-15)
		// to put color with IRC protocole (invisible in terminal)
		ss << "" << getNDigitCode(i, 2) << data << "\n";
		i = (i + 1) % 16; 
	}
	ifs.close();
	return ss.str();
}
void    IRC::MOTD( const Cmd & cmd, vector<t_ClientMsg> & res ) {
	string motd = ReadMOTD();

	//No need to manage ERR_NOSUCHSERVER (402) - because ONLY ONE server in this project
	//also, no need to manage [<target>]

	//if MOTD exist
	if (!motd.empty()) {
		PushToRes(cmd._user->_fd, getServReply(cmd._user, RPL_MOTDSTART, NULL), res);
		PushToRes(cmd._user->_fd, getServReply(cmd._user, RPL_MOTD, (string[]){ motd }), res);
		PushToRes(cmd._user->_fd, getServReply(cmd._user, RPL_ENDOFMOTD, NULL), res);
	} else // NOMOTD
		PushToRes(cmd._user->_fd, getServReply(cmd._user, ERR_NOMOTD, NULL), res);
}