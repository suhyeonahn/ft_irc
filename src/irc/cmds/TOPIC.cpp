#include "IRC.hpp"

//FIXME: put error case etc
// ERR_NEEDMOREPARAMS (461)
// ERR_NOSUCHCHANNEL (403)
// ERR_NOTONCHANNEL (442)
// ERR_CHANOPRIVSNEEDED (482)
// RPL_NOTOPIC (331)
// RPL_TOPIC (332)
// RPL_TOPICWHOTIME (333)
void IRC::TOPIC( const Cmd &cmd, vector<t_ClientMsg> & res) {
    string  servReply;
    Channel *chan;

    //ERROR RPLY
    
    //No error    
    chan = _chanList[cmd._params[0]];
    servReply = getServReply(cmd._user, RPL_TOPIC, (string[]){chan->getName(),chan->getTopic()});

    if (!servReply.empty())
        PushToRes(cmd._user->getFd(), servReply, res);
}


