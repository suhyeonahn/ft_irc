#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "ParseCmd.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Numerics.hpp"
# include "IRC.hpp"
//  All the client CMDS will be treated in this Class
//  to categorize the CMDS
//  Possible Categories :
//      Connection CMDS / Channel Operations / Server Queries and CMDS / Seding MSGS etc..
class   Cmd
{
    
    private:

        string                  _cmd;
	    vector<string>          _params;
        User *                  _user;


    public:
        Cmd( 
            string const & cmd,
            vector<string> params,
            User * user
        );
	    virtual ~Cmd();

        friend class IRC;
};

#endif