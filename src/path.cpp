#include "path.hpp"
#include <msfl.hpp>
#include <msutil.hpp>
#include <vector>

std::string Path::project_dir = "";

//cross platform pathing
/*std::string Path::GetOSPath(std::string path) {
    std::vector<std::string> pathItems = SplitString(path, '/');
    std::string out = "";
    #ifdef WIN32
        const char pDelim = '\\';
    #else
        const char pDelim = '/';
    #endif
    size_t _ = 0;
    for (const std::string i : pathItems) {
        if (i.length() <= 0) {
            if (_++ > 0)
                return "Invalid Path!";
            else
                continue;
        }

        out += i + pDelim;
    }

    return out;
}*/