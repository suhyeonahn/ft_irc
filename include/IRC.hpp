//TODO: IRC class to process cmd sent by client and to response to irc client
#ifndef IRC_HPP
# define IRC_HPP

# include "Preset.hpp"
# include "Client.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "ParseCmd.hpp"
# include "Cmd.hpp"
# include "Utils.hpp"
# include "Numerics.hpp"

class   IRC
{
    private:

        string const                   _pw; // password
        map<int, User *>               _userList;
        map<std::string, Channel *>    _channelList;

    public:
        IRC(const string &pw);
        virtual ~IRC();

        bool    ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res);
};

#endif