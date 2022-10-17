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

        // Channel mode
        set<User *>     _operList;  // Channel operators

    public:

        Channel( string const & name, User * creator );
        virtual ~Channel();

        static bool IsPrefix(char c);
        static bool IsValidName(const string &name);

    
        friend class Cmd;
        friend class IRC;
};

#endif