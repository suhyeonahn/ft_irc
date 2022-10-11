#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "User.hpp"
# include "Utils.hpp"

# define TOTAL_CMD 3
# define TOTAL_IMPLEMENTED_CMD 3

//  All the client CMDS will be treated in this Class
//  Divide them into several child classes might be a good idea
//  to categorize the CMDS
//  Possible Categories :
//      Connection CMDS / Channel Operations / Server Queries and CMDS / Seding MSGS etc..
class   Cmd
{
    
    private:

	    User *              _user;
        int                 _cmd;
	    std::vector<string> _params;
        std::string         _cmdList[TOTAL_CMD];

        void    setCmdList();
	    void	setCmd( string & msg );
	    void	setParams( string const & msg );
        
        // Connection cmds
        void    PASS();
        void    NICK();
        void    USER();

    public:

	    Cmd( User * user, string const & msg );
	    virtual ~Cmd();

	    bool	isValid() const;
	    bool	isImplemented() const;

        void    execute();

};

#endif