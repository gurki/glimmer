#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace glimmer {

class Frame;


struct TimedTrace {
    std::vector<std::string> callers;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
};


struct Collapse
{
    std::vector<TimedTrace> traces;
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    int depth;

    static Collapse fromFrame( const Frame& );
};


}   //  ::glimmer