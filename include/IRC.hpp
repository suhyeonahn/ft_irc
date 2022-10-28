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

        string const            _pw; // password
        map<int, User *>        _userList;
        map<string, Channel *>  _chanList;

        //  Connection cmds
        void    PASS( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    NICK( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    USER( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    QUIT( const Cmd &cmd, vector<t_ClientMsg> & res );
        //  Channel Operation cmds
        void    JOIN( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    TOPIC( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    NAMES( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    INVITE( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    PART( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    LIST( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    KICK( const Cmd &cmd, vector<t_ClientMsg> & res );
        //  Server Queries and cmds
        void    MODE( const Cmd &cmd, vector<t_ClientMsg> & res );
        //  Sending Messages
        void    PRIVMSG( const Cmd &cmd, vector<t_ClientMsg> & res );
        void    NOTICE( const Cmd &cmd, vector<t_ClientMsg> & res );
        //  User-Based Queries
        void    WHO( const Cmd &cmd, vector<t_ClientMsg> & res );
        //  Operator Messages
        void    KILL( const Cmd &cmd, vector<t_ClientMsg> & res );

        void    execute(const Cmd &cmd, vector<t_ClientMsg> & res );

        User            *getUserByNick( string const & nick ) const;
        Channel         *GetChannelByName ( const string &name ) const;
        Channel         *CreateChannel( const string &name, User *user);
		string	        Emit(User *user, string params[], const set<User *> &userList, vector<t_ClientMsg> &res, bool excludeUser);
        set<User *>     GetSameChanUsers(User *user);

        void            Emit2( User *user, const set<User *> & userList, string msg, vector<t_ClientMsg> & res, bool excludeUser );

        bool            ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res);
        void            DeleteOffUser(int fd);

        string  getServReply( User * user, int code, string params[] );
        string  getServMsg( User * user, int code, string params[] );
        string	get3DigitCode(int code);



    public:
        IRC(const string &pw);
        virtual ~IRC();


        void  test();

        void DEBUG();
        static void PRINT_USER_SET(set<User *> userset, const string &type);
        static void PRINT_STRING(const string &current, const string &type);
        
        static void            PushToRes( int fd, const string &msg, vector<t_ClientMsg> &res );


        friend class Server;


};

#endif