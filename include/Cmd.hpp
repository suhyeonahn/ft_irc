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
        map<int, User *>        &_userList;
        map<string, Channel *>  &_chanList;
        //put reference to synchronize _userList and _chanList in Cmd class with thoses things in IRC class



        //  Connection cmds
        void    PASS( vector<t_ClientMsg> & res );
        void    NICK( vector<t_ClientMsg> & res );
        void    USER( vector<t_ClientMsg> & res );
        //  Channel Operation cmds
        void    JOIN( vector<t_ClientMsg> & res );
        void    TOPIC( vector<t_ClientMsg> & res );
        void    NAMES( vector<t_ClientMsg> & res );
        void    INVITE( vector<t_ClientMsg> & res );
        void    PART( vector<t_ClientMsg> & res );
        void    LIST( vector<t_ClientMsg> & res );
        void    KICK( vector<t_ClientMsg> & res );
        //  Server Queries and cmds
        void    MODE( vector<t_ClientMsg> & res );
        //  User-Based Queries
        void    WHO( vector<t_ClientMsg> & res );

        void    QUIT( vector<t_ClientMsg> & res );

    public:
        Cmd( 
            string const & cmd,
            vector<string> params,
            User * user,
            map<int, User *> & userList,
            map<string, Channel *> & chanList
        );
	    virtual ~Cmd();

        User *           getUserByNick( string const & nick ) const;
        Channel         *GetChannelByName(const string &name) const;
        Channel         *CreateChannel(const string &name, User *user);

        void    execute( vector<t_ClientMsg> & res );

        //Put static to use without Cmd's instance
        //see line 40 in IRC.cpp - if (!isValid(cmdStr){...}) 
        static void    PushToRes( int fd, const string & msg, vector<t_ClientMsg> & res );
};

#endif