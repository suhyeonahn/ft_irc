#ifndef USER_HPP
# define USER_HPP

# include "Preset.hpp"

# define DEFAULT_NAME       "*"
# define VALID_CHARS_NICK   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"

class   User // Need to add an Operator class as a child class
{
    
    private:

        int             _fd;
        std::string     _nick; // Need to apply nickname restrictions
        std::string     _uname;
        std::string     _rname;

        bool            _isGoodPw;
        bool            _isRegistered;

        //std::string     _mode;

        void    setNick( std::string const & nick );
        void    setUname( std::string const & uname );
        void    setRname( std::string const & rname );

    public:

        User( int fd );
        virtual ~User();

        std::string    getNick()   const;
        std::string    getUname()  const;

        friend class Cmd;
};

#endif