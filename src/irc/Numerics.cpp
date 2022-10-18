//  Server Replies to Client 
//
//  used for both errors and normal replies
//  Distinct from a normal message, 
//  a numeric reply MUST contain a <source> and use a three-digit numeric as the command.
//  A numeric reply SHOULD contain the target of the reply as the first parameter of the message.
//  A numeric reply is not allowed to originate from a client.

#include "Numerics.hpp"

//Util function to add '0' until code string's length become 3
// ex: 1 => 001, 42 => 042, 242 => 242
string	get3DigitCode(int code) {
	string res = to_string(code);

	for(; res.length() < 3;) 
		res.insert(0, "0");
	return res;
}

string  getServReply(User *user, int code, string params[])
{
	stringstream	ss;

	// is it mandatory...?
	// put prefix
	// ex - ':<HOST> <CODE> <NICKNAME> '
	ss << ":" << HOST << " " << get3DigitCode(code) << " " << user->getNick() << " ";

	switch (code)
	{
		case RPL_WELCOME: // 001
			ss << ":Welcome to the FT_IRC42 Network, " << user->getNick(); break; 
		case RPL_TOPIC: // 332
			ss << ":" << params[0]; break;
		case RPL_NAMREPLY: // 353
			ss << params[0] << " :" << params[1]; break;
		case RPL_ENDOFNAMES: // 366
			ss << params[0] << " :End of /NAMES list"; break;
		case RPL_INVITING:	// 341
		{	
			ss << params[0] << " has been invited to " << params[1]; 
			if (*(&params + 1) - params == 3)
				ss << " by " << params[2];
			break;
		}
		case RPY_PART: // 999
			ss << "is leaving the channel " << params[0];
		case ERR_TOOMANYCHANNELS:
			ss << ":You have joined too many channels"; break;
		case ERR_UNKNOWNCOMMAND: // 421
			ss << params[0] << " :Unknown command"; break;
		case ERR_NEEDMOREPARAMS: // 461
			ss << params[0] << " :Not enough parameters"; break;
		case ERR_ALREADYREGISTERED: // 462
			ss << ":You may not reregister"; break;
		case ERR_PASSWDMISMATCH: // 464
			ss << ":Password incorrect"; break;
		case ERR_BADCHANMASK: //476
			ss << params[0] << " :Bad Channel Mask"; break;
		case ERR_NONICKNAMEGIVEN: // 431
			ss << ":No nickname given"; break;
        case ERR_ERRONEUSNICKNAME: // 432
            ss << params[0] << " :Erroneus nickname"; break;
        case ERR_NICKNAMEINUSE: // 433
            ss << params[0] << " :Nickname is already in use"; break;
		case ERR_NOSUCHCHANNEL: // 403
			ss << params[0] << " :No such channel"; break;
		case ERR_NOTONCHANNEL: // 442
			ss << params[0] << " :You're not on that channel"; break; 
		case ERR_USERONCHANNEL: // 443
			ss << params[0] << " " << params[1] << " :is already on channel"; break;
		case ERR_CHANOPRIVSNEEDED: // 482
			ss << params[0] << " :You're not channel operator"; break;
		case ERR_NOSUCHNICK: // (401)
			ss << params[0] << " :No such nick"; break; 
		default: break;

	}
	ss	<< SEP_MSG;

	return ss.str();
}


