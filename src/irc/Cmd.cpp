#include "Cmd.hpp"

Cmd::Cmd( User * user, string const & msg, std::string const & servPw ) 
    : _user(user), _servPw(servPw), _cmd(-1)
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

void    Cmd::PASS()
{
    if (_params.empty())
        ; // err numeric
    if (_params[0] == _servPw)
        _user->_isGoodPw = true;
}

void    Cmd::NICK()
{
    std::string const   &nick(_params[0]);
    if (nick == _user->getNick())
        return  ;
    else
        _user->setNick(nick);
}

void    Cmd::USER()
{
    if (_params.size() < 4)
        ; // err numeric
    else
    {
        _user->setUname(_params[0]);
        _user->setRname(_params[3]);
        _user->_isRegistered = true;
    }
}