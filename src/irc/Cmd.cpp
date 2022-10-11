#include "Cmd.hpp"

Cmd::Cmd( User * user, string const & msg ) : _user(user), _cmd(-1)
{
    string  cpyMsg(msg);
    setCmdList();
    setCmd(cpyMsg);
    if (isImplemented())
        setParams(cpyMsg);
}

Cmd::~Cmd() {}

void    Cmd::setCmdList()
{
    _cmdList[0] = "PASS";
    _cmdList[1] = "NICK";
    _cmdList[2] = "USER"; 
}

void	Cmd::setCmd( string & msg )
{
    size_t      i(msg.find(' ', 0));
    std::string tmp;
	if (i == string::npos)
    {
		tmp = msg;
        msg = "";
    }
	else
    {
		tmp = msg.substr(0, i);
        msg = msg.substr(i, msg.size() - i);
    }
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

    for (int i = 0; i < TOTAL_CMD; i++)
    {
        if ( _cmdList[i] == tmp)
            _cmd = i;
    }
}

void	Cmd::setParams( string const & msg )
{
    size_t  i(msg.find(" :", 0));
    if (i == std::string::npos)
        i = msg.size();
    
    std::string paramsStr(msg.substr(0, i));
    _params = ::split(paramsStr, " ");
}

bool	Cmd::isValid() const
{
    if (_cmd == -1)
        return false;
    return true;
}

bool	Cmd::isImplemented() const
{
    if (_cmd != -1 && _cmd < TOTAL_IMPLEMENTED_CMD)
        return true;
    return false;
}

void    Cmd::execute()
{
    switch ( _cmd )
    {
        case 0:  { PASS(); }
        case 1:  { NICK(); }
        case 2:  { USER(); }
        default:    ;
    }

}

void    Cmd::PASS() {}
void    Cmd::NICK() {}
void    Cmd::USER() {}