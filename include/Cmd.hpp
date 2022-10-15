#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Numerics.hpp"

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
        map<int, User *>        _userList;
        map<string, Channel *>  _chanList;

        // Connection cmds
        void    PASS( vector<t_ClientMsg> & res );
        void    NICK( vector<t_ClientMsg> & res );
        void    USER( vector<t_ClientMsg> & res );

    
    public:
        Cmd( string const & cmd, vector<string> params, User * user, map<int, User *> & userList );
	    virtual ~Cmd();

        User *  getUserByNick( string const & nick ) const;

        void    execute( vector<t_ClientMsg> & res );

        //Put static to use without Cmd's instance
        //see line 40 in IRC.cpp - if (!isValid(cmdStr){...}) 
        static void    PushToRes( int fd, const string & msg, vector<t_ClientMsg> & res );
};

#endif