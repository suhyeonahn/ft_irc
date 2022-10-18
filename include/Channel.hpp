#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Cmd.hpp"

# define CHAN_PREFIX '#'
# define CHAN_INVALID_CHAR "\a,: "


class   Channel
{
    
    private:

        string  const   &_name;
        set<User *>     _userList;  // joined users
        string          _topic;
        //  Channel mode
        bool            _i; //  inviteOnly
        set<User *>     _operList;  // Channel operators

        void    addUser( User * user );

    public:

        Channel( string const & name, User * creator );
        virtual ~Channel();

        static bool IsPrefix(char c);
        static bool IsValidName(const string &name);

        string  getName() const;
        string  getTopic() const;
        string  getNicks() const;
        size_t  getNusers() const;
    
        friend class Cmd;
        friend class IRC;
};

#endif