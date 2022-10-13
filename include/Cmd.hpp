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
        void    PASS(vector<t_ClientMsg> &res);
        void    NICK(vector<t_ClientMsg> &res);
        void    USER(vector<t_ClientMsg> &res);

        void    PushToRes(const string msg, vector<t_ClientMsg> &res);
    
    public:

	    Cmd( string const & cmd, vector<string> params, User *  user );
	    virtual ~Cmd();

        void    execute(vector<t_ClientMsg> &res);

};

#endif