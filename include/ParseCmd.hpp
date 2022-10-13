#ifndef PARSECMD_HPP
# define PARSECMD_HPP

# include "Preset.hpp"
# include "Utils.hpp"

# define TOTAL_CMD 3
# define TOTAL_IMPLEMENTED_CMD 3


void            initCmdList();
string			setCmd( string & msg );
vector<string>	setParams( string const & msg );

bool    isValid();
bool	isImplemented();

#endif