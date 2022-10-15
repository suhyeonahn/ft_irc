#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Preset.hpp"
# include "User.hpp"

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
    
        friend class IRC;
};

#endif