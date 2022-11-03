#ifndef USER_HPP
# define USER_HPP

# include "Preset.hpp"
# include "Channel.hpp"

# define DEFAULT_NAME       "*"
# define VALID_CHARS_NICK   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"
# define USER_MODE          "oi"

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

        void    setNick( string const & nick );
        void    setUname( string const & uname );
        void    setRname( string const & rname );
        void    setMode( bool plus, string const & mode );

        bool    isValidNick( string const & nick );
        bool    isValidMode( string const & mode );

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