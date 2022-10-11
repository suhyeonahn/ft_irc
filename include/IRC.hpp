//TODO: IRC class to process cmd sent by client and to response to irc client
#ifndef IRC_HPP
# define IRC_HPP

# include "Preset.hpp"
# include "Client.hpp"
# include "User.hpp"
# include "Cmd.hpp"

class   IRC
{
    private:

        std::map<int, User *>   _userList;
    
    public:

        IRC();
        virtual ~IRC();

        bool    ProcessClientMsg( t_ClientMsg const & msg );
};

#endif