#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Numerics.hpp"
# include "IRC.hpp"

# define TOTAL_CMD 3
# define TOTAL_IMPLEMENTED_CMD 3

class   Cmd
{
    
    private:
        string          _cmd;
	    vector<string>  _params;
        User *          _user;

        void		setCmd( string & cpyMsg );
        void		setParams( string & cpyMsg );

        bool    isValid();
        bool	isImplemented();

    public:
        Cmd( User * user, const string & msg );
	    virtual ~Cmd();

    	static void        initCmdList();

        friend class IRC;
};

#endif