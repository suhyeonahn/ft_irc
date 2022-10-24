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


    public:
        IRC(const string &pw);
        virtual ~IRC();

        bool            ProcessClientMsg( t_ClientMsg const & msg, vector<t_ClientMsg> &res);
        //put static and send userList, chanList because I have to use this function in Server class and Cmd class
        static void         DeleteOffUser(int fd, map<int, User *> &userList, map<string, Channel *> &chanList);
		static string	    Emit(User *user, string params[], set<User *> userList, vector<t_ClientMsg> &res, bool excludeUser);
        static set<User *>  GetSameChanUsers(User *user);




        void  test();

        void DEBUG();
        static void PRINT_USER_SET(set<User *> userset, const string &type);
        static void PRINT_STRING(const string &current, const string &type);

        friend class Server;


};

#endif