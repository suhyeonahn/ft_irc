#ifndef CMD_HPP
# define CMD_HPP

# include "Preset.hpp"
# include "ParseCmd.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include "Numerics.hpp"
# include "IRC.hpp"

class   Cmd
{
    
    private:

        string                  _cmd;
	    vector<string>          _params;
        User *                  _user;


    public:
        Cmd( 
            string const & cmd,
            vector<string> params,
            User * user
        );
	    virtual ~Cmd();

        friend class IRC;
};

#endif