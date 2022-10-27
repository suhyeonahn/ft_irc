#ifndef PARSECMD_HPP
# define PARSECMD_HPP

# include "Preset.hpp"
# include "Utils.hpp"

# define TOTAL_CMD 3
# define TOTAL_IMPLEMENTED_CMD 3


void            initCmdList();
string			setCmd( string & msg );
vector<string>	setParams( string & cmd, string const & msg );

bool    isValid( string const & cmd );
bool	isImplemented( string const & cmd );


#endif