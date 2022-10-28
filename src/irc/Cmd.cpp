#include "Cmd.hpp"

static set<string>	ignoredCmd;
static set<string>	implementedCmd;

Cmd::Cmd( User * user, const string & msg ) : _user(user)
{
    string  cpyMsg(msg);



    setCmd(cpyMsg);
    if (isImplemented())
    {
        setParams(cpyMsg);
    }
}

Cmd::~Cmd() {}

void	Cmd::initCmdList()
{
	const char *ignored[] =
	{
		"CAP",
		"CNOTICE",
		"CPRIVMSG",
		"CONNECT",
		"ENCAP",
		"ERROR",
		"HELP",
		"INFO",
		"ISON",
		"KNOCK",
		"LINKS",
		"LUSERS",
		"NAMESX",
		"PONG",
		"REHASH",
		"RULES",
		"SERVER",
		"SERVICE",
		"SERVLIST",
		"SQUERY",
		"SQUIT",
		"SETNAME",
		"SILENCE",
		"STATS",
		"SUMMON",
		"TRACE",
		"UHNAMES",
		"USERHOST",
		"USERIP",
		"USERS",
		"WALLOPS",
		"WATCH",
		"WHOIS",
		"WHOWAS",
		NULL
	};
	const char *implemented[] =
	{
		"ADMIN",
		"AWAY",
		"DIE",
		"INVITE",
		"JOIN",
		"KICK",
		"KILL",
		"LIST",
		"MODE",
		"MOTD",
		"NAMES",
		"NICK",
		"NOTICE",
		"OPER",
		"PART",
		"PASS",
		"PING",
		"PRIVMSG",
		"QUIT",
		"TIME",
		"TOPIC",
		"USER",
		"VERSION",
		"WHO",
		NULL
	};

	for (int i(0); ignored[i]; ++i)	
		ignoredCmd.insert(ignored[i]);
	for (int i(0); implemented[i]; ++i)	
		implementedCmd.insert(implemented[i]);
}

void    Cmd::setCmd( string & cpyMsg )
{
    size_t  i(cpyMsg.find(' ', 0));

	if (i == string::npos)
    {
		_cmd = cpyMsg;
        cpyMsg = "";
    }
	else
    {
		_cmd = cpyMsg.substr(0, i);
        cpyMsg = cpyMsg.substr(i, cpyMsg.size() - i);
    }
}

void Cmd::setParams( string & cpyMsg )
{
    size_t  i(cpyMsg.find(" :", 0));
    if (i == string::npos)
        i = cpyMsg.size();
    
    string paramsStr(cpyMsg.substr(0, i));
    _params = ::split(paramsStr, " ");
    // cout << RED << "params[0]" << params[0] << DFT << endl;

	//	move index after " :"
	//	execept TOPIC
	if (_cmd == "TOPIC")
		i+= 1;
	else
		i += 2;
	if (i < cpyMsg.size())
		// last_param.size = all msg.size() - (before_last_param).size()
		_params.push_back(cpyMsg.substr(i, cpyMsg.size() - i));
    
}

bool    Cmd::isValid( void )
{
    if (_cmd.empty())
        return false;
    return (ignoredCmd.find(_cmd) != ignoredCmd.end())
		|| (implementedCmd.find(_cmd) != implementedCmd.end());
}

bool	Cmd::isImplemented( void )
{
    if (_cmd.empty())
		return false;
	return implementedCmd.find(_cmd) != implementedCmd.end();
}

