#ifndef UTILS_HPP
# define UTILS_HPP

# include "Preset.hpp"

vector<string> split( std::string const & str, std::string const delim );
vector<string> splitModeStr( std::string const & str, std::string const delim );
string  intToStr(int x);

#endif