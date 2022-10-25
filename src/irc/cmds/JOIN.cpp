#include "IRC.hpp"

// JOIN(client) - resp(server) -> MODE(client) -resp(server) -> WHO(client)...
void    IRC::JOIN( const Cmd & cmd, vector<t_ClientMsg> & res ) {
    string  servReply;
    Channel *chan;

    //ERROR_RPLY
    if (cmd._params.size() < 1) {
        servReply = getServReply(cmd._user, ERR_NEEDMOREPARAMS, (string[]){ cmd._cmd });
        PushToRes(cmd._user->_fd, servReply, res);
        return ;
    }

    vector<string> names;
    vector<string> keys;
    names = split(cmd._params[0], ",");
    if (cmd._params.size() > 1)
        keys = split(cmd._params[1], ",");
    
    for (int i = 0; i < names.size(); ++i) {
        const string &name = names[i];
        const string &key = (i < keys.size()) ? keys[i] : "";

        if (!Channel::IsValidName(name)) {
            servReply = getServReply(cmd._user, ERR_BADCHANMASK, (string []) { name });
            PushToRes(cmd._user->_fd, servReply, res);
            continue;
        }

        //TODO: modify here...
        chan = GetChannelByName(name);
        cout << chan << endl;
        if (chan == NULL)
            chan = CreateChannel(name, cmd._user);
        else {
            //if user not yet joined
            if (cmd._user->_joined.find(chan) == cmd._user->_joined.end()) {
                // if invite mode and user not invited
                if (chan->_i && chan->_invitedList.find(cmd._user) == chan->_invitedList.end())
                    servReply = getServReply(cmd._user, ERR_INVITEONLYCHAN, (string []) { name });
                // if channal has key and invalid key
                else if (!chan->_key.empty() && chan->_key != key)
                    servReply = getServReply(cmd._user, ERR_BADCHANNELKEY, (string[]) { name });
                else {
                    //if all ok
                    //add user 
                    chan->addUser(cmd._user);
                    //remove user in invite list
                    chan->_invitedList.erase(cmd._user);
                    //add channel to user
                    cmd._user->_joined.insert(chan);
                }
            }
        }
        if (!servReply.empty())
            PushToRes(cmd._user->_fd, servReply, res);
        else {
            //1. Emit msg to all user in channel (including current user)
            IRC::Emit(cmd._user, (string []) { "JOIN", name, "" }, chan->_userList, res, false);
            //2.topic(NOT YET)
            if (chan->_topic.size()) 
                TOPIC(cmd, res);
            //3. send names
            NAMES(cmd, res);
        }
    }
}