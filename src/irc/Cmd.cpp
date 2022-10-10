#include "Cmd.hpp"

Cmd::Cmd( User * user, string const & cmdMsg ) : _user(user), _cmdMsg(cmdMsg)
{
    _cmd = -1;
    setCmdList();
    setCmd();
    setParams();
}

void    Cmd::setCmdList()
{
    _cmdList[0] = "PASS";
    _cmdList[1] = "NICK";
    _cmdList[2] = "USER"; 
}

void	Cmd::setCmd()
{
    size_t      i(_cmdMsg.find(' ', 0));
    std::string tmp;
	if (i == string::npos)
		tmp = _cmdMsg;
	else
		tmp = _cmdMsg.substr(0, i);
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);


    for (int i = 0; i < TOTAL_CMD; i++)
    {
        if ( _cmdList[i] == tmp)
            _cmd = i;
    }
}

void	Cmd::setParams() {}

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