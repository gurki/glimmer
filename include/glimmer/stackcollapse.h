#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace glimmer {


class Profile;


struct StackTrace {
    std::vector<std::string> stack;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
};


struct StackCollapse {
    std::chrono::system_clock::time_point start;
    std::vector<StackTrace> traces;
};


StackCollapse foldStack( const Profile& );


}   //  ::glimmer