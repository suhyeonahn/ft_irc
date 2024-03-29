#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Cmd.hpp"

# define CHAN_PREFIX '#'
# define CHAN_INVALID_CHAR "\a,: "
# define OPER_PREFIX '@'
# define CHAN_MODE  "ik"

class   Channel
{
	
	private:

        string  const   _name;
        set<User *>     _userList;  // joined users
        string          _topic;

        string          _key; // Channel's password
        set<User *>     _operList;  // Channel operators
        set<User *>     _invitedList; // Invited Users in Channel

        //  Channel mode
        bool    _i; //  Invite-Only Channel Mode
        bool    _k; //  Key Channel Mode

        void    setMode( bool plus, string const & mode );

        void    addUser( User * user );
        void    rmUser( User * user );

	public:

		Channel( string const & name, User * creator );
		virtual ~Channel();


        string  getName() const;
        string  getTopic() const;
        string  getNicks( bool i ) const;
        size_t  getNusers() const;
        string  getMode()  const;
        User *  getOperByNick( string const & nick ) const;

		static bool IsPrefix(char c);
		static bool IsValidName(const string &name);
        bool    isValidMode( string const & mode );

		friend class IRC;
};

#endif