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
string	IRC::get3DigitCode(int code) {
	string res = intToStr(code);

	for(; res.length() < 3;) 
		res.insert(0, "0");
	return res;
}

string  IRC::getServReply( User * user, int code, string params[] )
{
	stringstream	ss;

	// is it mandatory...?
	// put prefix
	// ex - ':<HOST> <CODE> <NICKNAME>[!user@host] '
	ss << ":" << SERV_HOST << " " << get3DigitCode(code) << " "
		<< user->getNick() << " ";

	switch (code)
	{
		case RPL_WELCOME: // 001
			ss << ":Welcome to the FT_IRC42 Network, " << user->getNick(); break; 
		case RPL_NAMREPLY: // 353  "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
			ss << params[0] << " :" << params[1]; break;
		case RPL_ENDOFNAMES: // 366
			ss << params[0] << " :End of /NAMES list"; break;
		case RPL_INVITING:	// 341
			ss << params[0] << " :has been invited to " << params[1]; break;
		case RPL_AWAY: // 301  FIXEME :: message is a pram "<client> <nick> :<message>"
			ss << params[0] << ":is away" ; break;
		case RPL_LISTSTART: // 321
			ss << "Channel :Users  Name"; break;
		case RPL_LIST: // 322
		//  <channel> <client count> :<topic>
			ss << params[0] << " " << params[1] << " :" << params[2]; break;
		case RPL_LISTEND: //323
			ss << ":End of /LIST"; break;
		case RPL_UMODEIS:	//	221
			ss << params[0]; break;
		case RPL_CHANNELMODEIS:	//	324
			ss << params[0] << " " << params[1]; break;
		case RPL_WHOREPLY:	//352
			ss << params[0]; break;
		case RPL_ENDOFWHO:	//315
			ss << params[0] << " :End of WHO list"; break;
		case RPL_NOTOPIC: //331
			ss << params[0] << " :No topic is set"; break;
		case RPL_TOPIC: //332
			ss << params[0] << " :" << params[1]; break;
		case RPL_TOPICWHOTIME: //323
			ss << params[0] << " " << params[1] << " " << getTime(); break;
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
		case ERR_NOSUCHNICK: // 401
			ss << params[0] << " :No such nick"; break;
		case ERR_USERSDONTMATCH: // 502
			ss << ":Cant change mode for other users"; break;
		case ERR_UMODEUNKNOWNFLAG: // 501
			ss << ":Unknown MODE flag"; break;
		case ERR_NORECIPIENT: // 411
			ss << ":No recipient given"; break;
		case ERR_NOTEXTTOSEND: // 412
			ss << ":No text given to send"; break;
		default: break;
	}
	ss	<< SEP_MSG;

	return ss.str();
}

string  IRC::getServMsg( User * user, int code, string params[] )
{
	stringstream	ss;
	
	// put prefix(source)
	// ex - ':<HOST> <NICKNAME>[!user@host] '
	// ss << ":" << SERV_HOST << " "
	// 	<< user->getNick() << "!" << user->getUname() << "@" << USR_HOST << " ";
	ss << ":" << user->getNick() << " ";
	switch (code)
	{
		case MSG_PART: // 1000
		{
			ss  << "PART " << params[0];
			if (*(&params + 1) - params == 2)
			ss << " :" << params[1];
			break;
		}
		case MSG_KICK: // 1001
			ss  << "KICK " << params[0] << " " << params[1] << " :" << params[2]; break;
		case MSG_PRIVMSG:	//	1002
			ss  << "PRIVMSG " << params[0] << " :" << params[1]; break;
		case MSG_NOTICE:	//	1003
			ss  << "NOTICE " << params[0] << " :" << params[1]; break;
		case MSG_INVITE:	//	1004
			ss  << "INVITE " << params[0] << " " << params[1]; break;
		case MSG_ERROR:	//1005
			ss  << "ERROR :" << params[0]; break;
		case MSG_QUIT:	//1006
			ss  << "QUIT :" << params[0]; break;
	}
	ss << SEP_MSG;

	return ss.str();
}

