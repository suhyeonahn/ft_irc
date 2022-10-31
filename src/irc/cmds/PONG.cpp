#include "IRC.hpp"

void    IRC::PONG( const Cmd & cmd, vector<t_ClientMsg> & res )
{
	stringstream token;

	token << ":" << SERV_HOST << " ";
	if (cmd._params.empty())
		token << get3DigitCode(ERR_NOORIGIN) << " :No origin specified";
	else
		token << "PONG " << SERV_HOST << " :" << cmd._params[0];
	token << SEP_MSG;
	PushToRes(cmd._user->_fd, token.str(), res);
}
