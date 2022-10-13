#include "ParseCmd.hpp"

static std::set<string>	ignoredCmd;
static std::set<string>	implementedCmd;

void	initCmdList()
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

string  setCmd( string & msg )
{
    size_t  i(msg.find(' ', 0));
    string  cmd;

	if (i == string::npos)
    {
		cmd = msg;
        msg = "";
    }
	else
    {
		cmd = msg.substr(0, i);
        msg = msg.substr(i, msg.size() - i);
    }
    
    return cmd;
}

vector<string>  setParams( string const & msg )
{
    size_t  i(msg.find(" :", 0));
    if (i == string::npos)
        i = msg.size();
    
    string paramsStr(msg.substr(0, i));
    vector<string>  params = ::split(paramsStr, " ");
    
    return params;
}

bool    isValid( string const & cmd )
{
    if (cmd.empty())
        return false;
    return (ignoredCmd.find(cmd) != ignoredCmd.end())
		|| (implementedCmd.find(cmd) != implementedCmd.end());
}

bool	isImplemented( string const & cmd )
{
    if (cmd.empty())
		return false;
	return implementedCmd.find(cmd) != implementedCmd.end();
}