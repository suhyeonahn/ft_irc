#include "IRC.hpp"

void    IRC::MODE( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    string  servReply;
    //  Check if _params[0] is a nick or a chan
    if (cmd._params[0][0] == CHAN_PREFIX)
    {
        //  Target is a chan
        Channel * chan = GetChannelByName(cmd._params[0]);
        //  chan not on the server
        if (chan == NULL)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NOSUCHCHANNEL, (string[]){cmd._params[0]}), res);
        //  modestring is not given then send a msg containing the current modes
        else if (cmd._params.size() < 2)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_CHANNELMODEIS, (string[]){chan->getName(),chan->getMode()}), res);
        else
        {
            //  ERR if the user is not a channel operator.
            if (chan->_operList.find(cmd._user) == chan->_operList.end())
				PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_CHANOPRIVSNEEDED, (string[]){chan->getName()}), res);
            else
            {
                //  modestring is given
                bool isValid = true;
                bool plus;
                //  parse modestring
                vector<string> modeStr = splitModeStr(cmd._params[1], "+-");
                for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
                {
                    string token = *it;
                    if (token[0] != '+' && token[0] != '-')
                        servReply = getServReply(cmd._user, ERR_UMODEUNKNOWNFLAG, NULL); //461
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
                                servReply = getServReply(cmd._user, ERR_UMODEUNKNOWNFLAG, NULL); //461
                        }
                    }
                }
                //  Send changed mode
                PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_UMODEIS, (string[]){chan->getMode()}), res);
            }
        }
    }
    else
    {
        //  Target is a usr
        User * usr = getUserByNick(cmd._params[0]);
        //  usr not on the server
        if (usr == NULL)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOSUCHNICK, (string[]){cmd._params[0]}), res);
        //  cmd._user is not matching the nick
        else if (cmd._user != usr)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_USERSDONTMATCH, NULL), res);
        //  modestring is not given then send a msg containing the current modes
        else if (cmd._params.size() < 2)
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_UMODEIS, (string[]){cmd._user->getMode()}), res);
        else
        {
            //  modestring is given
            bool isValid = true;
            bool plus;
            //  parse modestring
            vector<string> modeStr = splitModeStr(cmd._params[1], "+-");
            for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
            {
                string token = *it;
                if (token[0] != '+' && token[0] != '-')
                    servReply = getServReply(cmd._user, ERR_UMODEUNKNOWNFLAG, NULL); //461
                else
                {
                    if (token[0] == '+')
                        plus = true;
                    else if (token[1] == '-')
                        plus = false;
                    for(string::size_type i = 1; i < token.size(); ++i)
                    {
                        if (isValid == cmd._user->isValidMode(token[i]))
                            cmd._user->setMode(plus, token[i]);
                        if (!isValid)
                            servReply = getServReply(cmd._user, ERR_UMODEUNKNOWNFLAG, NULL); //461
                    }
                }
            }
            //  Send changed mode
            PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_UMODEIS, (string[]){cmd._user->getMode()}), res);
        }
    }
    if (!servReply.empty())
        PushToRes(cmd._user->getFd(), servReply, res);
}