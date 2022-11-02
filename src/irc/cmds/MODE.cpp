#include "IRC.hpp"

void    IRC::MODE( const Cmd & cmd, vector<t_ClientMsg> & res )
{
    bool unknown = false;
    string  servReply;
    if (cmd._params.empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user,  ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
    //  Check if _params[0] is a nick or a chan
    else if (cmd._params[0][0] == CHAN_PREFIX)
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
                //  parse modestring
                vector<string> modeStr = splitModeStr(cmd._params[1], "+-");
                for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
                {
                    string token = *it;
                    if ((token[0] != '+' && token[0] != '-') || token.size() == 1)
                        unknown = true;
                    else
                    {
                        bool plus = token[0] == '+' ? true : false;
                        if (cmd._user->isValidMode(&token[1]) == false)
                            unknown = true;
                        chan->setMode(plus, &token[1]);
                        servReply = getServReply(cmd._user, RPL_CHANNELMODEIS, (string[]){chan->getName(),chan->getMode()});
                    }
                }
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
            //  parse modestring
            vector<string> modeStr = splitModeStr(cmd._params[1], "+-");
            for (vector<string>::iterator it = modeStr.begin() ; it != modeStr.end() ; ++it)
            {
                string token = *it;
                if ((token[0] != '+' && token[0] != '-') || token.size() == 1)
                    unknown = true;
                else
                {
                    bool plus = token[0] == '+' ? true : false; 
                    if (cmd._user->isValidMode(&token[1]) == false)
                        unknown = true;
                    cmd._user->setMode(plus, &token[1]);
                    servReply = getServReply(cmd._user, RPL_UMODEIS, (string[]){cmd._user->getMode()});
                }
            }
        }
    }
    if (!servReply.empty())
        PushToRes(cmd._user->getFd(), servReply, res);
    if (unknown == true)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_UMODEUNKNOWNFLAG, NULL), res); //461
}