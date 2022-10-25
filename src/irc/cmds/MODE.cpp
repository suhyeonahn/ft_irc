#include "IRC.hpp"

void    IRC::MODE( const Cmd &cmd, vector<t_ClientMsg> & res )
{
    string  servReply;
    //  Check if _params[0] is a nick or a chan
    if (_params[0][0] == CHAN_PREFIX)
    {
        //  Target is a chan
        Channel * chan = GetChannelByName(_params[0]);
        //  chan not on the server
        if (chan == NULL)
            PushToRes(_user->_fd, getServReply(_user,  ERR_NOSUCHCHANNEL, (string[]){_params[0]}), res);
        //  modestring is not given then send a msg containing the current modes
        else if (_params.size() < 2)
            PushToRes(_user->_fd, getServReply(_user, RPL_CHANNELMODEIS, (string[]){chan->getName(), chan->getMode()}), res);
        else
        {
            //  ERR if the user is not a channel operator.
            if (chan->_operList.find(_user) == chan->_operList.end())
				PushToRes(_user->_fd, getServReply(_user, ERR_CHANOPRIVSNEEDED, (string[]){chan->getName()}), res);
            else
            {
                //  modestring is given
                bool isValid = true;
                bool plus;
                //  parse modestring
                vector<string> modeStr = splitModeStr(_params[1], "+-");
                for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
                {
                    string token = *it;
                    if (token[0] != '+' && token[0] != '-')
                        servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                    else
                    {
                        if (token[0] == '+')
                            plus = true;
                        else if (token[1] == '-')
                            plus = false;
                        for(string::size_type i = 1; i < token.size(); ++i)
                        {
                            if (isValid == chan->isValidMode(token[i]))
                                chan->setMode(plus, token[i]);
                            if (!isValid)
                                servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                        }
                    }
                }
                //  Send changed mode
                PushToRes(_user->_fd, getServReply(_user, RPL_UMODEIS, (string[]){chan->getMode()}), res);
            }
        }
    }
    else
    {
        //  Target is a usr
        User * usr = getUserByNick(_params[0]);
        //  usr not on the server
        if (usr == NULL)
            PushToRes(_user->_fd, getServReply(_user, ERR_NOSUCHNICK, (string[]){_params[0]}), res);
        //  _user is not matching the nick
        else if (_user != usr)
            PushToRes(_user->_fd, getServReply(_user, ERR_USERSDONTMATCH, NULL), res);
        //  modestring is not given then send a msg containing the current modes
        else if (_params.size() < 2)
            PushToRes(_user->_fd, getServReply(_user, RPL_UMODEIS, (string[]){_user->getMode()}), res);
        else
        {
            //  modestring is given
            bool isValid = true;
            bool plus;
            //  parse modestring
            vector<string> modeStr = splitModeStr(_params[1], "+-");
            for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
            {
                string token = *it;
                if (token[0] != '+' && token[0] != '-')
                    servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                else
                {
                    if (token[0] == '+')
                        plus = true;
                    else if (token[1] == '-')
                        plus = false;
                    for(string::size_type i = 1; i < token.size(); ++i)
                    {
                        if (isValid == _user->isValidMode(token[i]))
                            _user->setMode(plus, token[i]);
                        if (!isValid)
                            servReply = getServReply(_user, ERR_UMODEUNKNOWNFLAG, (string[]){ _cmd }); //461
                    }
                }
            }
            //  Send changed mode
            PushToRes(_user->_fd, getServReply(_user, RPL_UMODEIS, (string[]){_user->getMode()}), res);
        }
    }
    if (!servReply.empty())
        PushToRes(_user->_fd, servReply, res);
}