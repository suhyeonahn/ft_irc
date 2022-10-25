#ifndef USER_HPP
# define USER_HPP

# include "Preset.hpp"
# include "Channel.hpp"

# define DEFAULT_NAME       "*"
# define VALID_CHARS_NICK   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"
# define USER_MODE          "io"

class   User
{
    
    private:

        int             _fd;
        string  const   _servPw;
        string          _nick; // Need to apply nickname restrictions
        string          _uname;
        string          _rname;

        bool            _isGoodPw;
        bool            _isRegistered; // replace to _r ?

        set<Channel *>  _joined;

        string  _away; //  Away status: H(here) or G(gone)
        // User mode
        bool    _i; //  invisible
        bool    _o; //  oper

        void    setNick( std::string const & nick );
        void    setUname( std::string const & uname );
        void    setRname( std::string const & rname );
        void    setMode( bool plus, char const & mode );

        bool    isValidNick( string const & nick );
        bool    isValidMode( char const & mode );

        void    join( Channel * chan );

    public:

        User( int fd, string const servPw );
        virtual ~User();

        string  getNick()   const;
        string  getUname()  const;
        string  getMode()  const;
        string  getWho(const string chan) const;
        // string  getJoinedChan() const;
        int     getFd()  const;

        friend class IRC;
        friend class Channel;

};

#endif