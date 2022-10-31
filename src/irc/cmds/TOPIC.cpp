#include "IRC.hpp"

void IRC::TOPIC( const Cmd &cmd, vector<t_ClientMsg> & res)
{
    // if (!cmd._params[0].empty())
    //     cout << "0:" << cmd._params[0] << endl;
    // if (!cmd._params[1].empty())
    //     cout << "1:" << cmd._params[1] << endl;
    //ERROR RPLY
    if (cmd._params.empty())
    {
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){cmd._cmd}), res);
        return ;
    }
    Channel * chan = GetChannelByName(cmd._params[0]);
    //  chan not on the server
    if (chan == NULL)
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOSUCHCHANNEL, (string[]){cmd._params[0]}), res);
        //  ERR if the user is not a member of the channer
    else if (cmd._user->_joined.find(chan) == cmd._user->_joined.end())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_NOTONCHANNEL, (string[]){chan->_name}), res);
        //  ERR if the user is not a channel operator.
    else if (cmd._params.size() < 2 && chan->getTopic().empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_NOTOPIC, (string[]){chan->_name}), res);
    else if (cmd._params.size() < 2 && !chan->getTopic().empty())
        PushToRes(cmd._user->getFd(), getServReply(cmd._user, RPL_TOPIC, (string[]){chan->_name, chan->_topic}), res);
    else if (chan->_operList.find(cmd._user) == chan->_operList.end())
		PushToRes(cmd._user->getFd(), getServReply(cmd._user, ERR_CHANOPRIVSNEEDED, (string[]){chan->_name}), res);
    else
    {
        if (cmd._params[1] == ":")
            chan->_topic = "";
        else
            chan->_topic = cmd._params[1];
        Emit2(cmd._user, chan->_userList, getServReply(cmd._user, RPL_TOPIC, (string[]){chan->_name, chan->_topic}), res, false);
        Emit2(cmd._user, chan->_userList, getServReply(cmd._user, RPL_TOPICWHOTIME, (string[]){chan->_name, cmd._user->_nick}), res, false);
    }
}

