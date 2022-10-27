#include "Utils.hpp"

vector<string> split( std::string const & str, std::string const delim )
{
    vector<string> tokens;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        tokens.push_back(str.substr(start, end - start));
    }
    return tokens;
}

vector<string> splitModeStr( std::string const & str, std::string const delim )
{
    vector<string> tokens;
    size_t start;
    size_t end = 0;
    if ((start = str.find_first_of(delim, end)) != 0)
        tokens.push_back(str.substr(0, start));
    while ((start = str.find_first_of(delim, end)) != string::npos)
    {
        end = str.find_first_of(delim, start + 1);

        tokens.push_back(str.substr(start, end - start));
    }
    return tokens;
}

string  intToStr(int x)
{
    stringstream ss;
    ss << x;
    string str = ss.str();
    return str;
}

string  getTime()
{
    time_t result = time(NULL);
    return (asctime(localtime(&result)));
}
