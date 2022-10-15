//  Server Replies to Client 
//
//  used for both errors and normal replies
//  Distinct from a normal message, 
//  a numeric reply MUST contain a <source> and use a three-digit numeric as the command.
//  A numeric reply SHOULD contain the target of the reply as the first parameter of the message.
//  A numeric reply is not allowed to originate from a client.

#include "Numerics.hpp"

string  getServReply(User *user, int code, string params[])
{
	stringstream	ss;

	switch (code)
	{

		case ERR_NEEDMOREPARAMS: // 461
			ss  << params[0] << " :Not enough parameters"; break;
		case ERR_ALREADYREGISTERED: // 462
			ss  << " :You may not reregister"; break;
		case ERR_PASSWDMISMATCH: // 464
			ss  << " :Password incorrect"; break;

		case ERR_NONICKNAMEGIVEN: // 431
			ss  << " :No nickname given"; break;
        case ERR_ERRONEUSNICKNAME: // 432
            ss  << params[0] << " :Erroneus nickname"; break;
        case ERR_NICKNAMEINUSE: // 433
            ss  << params[0] << " :Erroneus nickname"; break;

		default: break;

	}
	ss	<< SEP_MSG;

	return ss.str();
}


