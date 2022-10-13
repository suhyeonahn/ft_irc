#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Utils.hpp"

//  All the client CMDS will be treated in this Class
//  to categorize the CMDS
//  Possible Categories :
//      Connection CMDS / Channel Operations / Server Queries and CMDS / Seding MSGS etc..
class   Cmd
{
    
    private:

        string          _cmd;
	    vector<string>  _params;
        User *          _user;

        // Connection cmds
        void    PASS();
        void    NICK();
        void    USER();
    
    public:

	    Cmd( string const & cmd, vector<string> params, User *  user );
	    virtual ~Cmd();

        void    execute();

};

#endif