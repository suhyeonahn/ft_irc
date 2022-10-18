#ifndef USER_HPP
# define USER_HPP

# include "Preset.hpp"
# include "Channel.hpp"

# define DEFAULT_NAME       "*"
# define VALID_CHARS_NICK   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"

class   User
{
    
    private:

        int             _fd;
        string  const   _servPw;
        string          _nick; // Need to apply nickname restrictions
        string          _uname;
        string          _rname;

        bool            _isGoodPw;
        bool            _isRegistered;

        set<Channel *>  _joined;

        // User mode
        bool            _servOper;

        void    setNick( std::string const & nick );
        void    setUname( std::string const & uname );
        void    setRname( std::string const & rname );

        bool    isValidNick( string const & nick );

        void    join( Channel * chan );

    public:

        User( int fd, string const servPw );
        virtual ~User();

        string    getNick()   const;
        string    getUname()  const;
        int    getFd()  const;

        friend class Cmd;

};

#endif