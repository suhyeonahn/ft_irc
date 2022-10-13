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

class   IRC
{
    private:

        std::string const                   _pw; // password
        std::map<int, User *>               _userList;
        std::map<std::string, Channel *>    _channelList;

    public:

        IRC();
        virtual ~IRC();

        bool    ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res);
};

#endif