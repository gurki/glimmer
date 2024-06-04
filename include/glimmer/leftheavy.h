#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace glimmer {

class Collapse;


struct CountedTrace {
    std::string name;
    std::vector<std::string> trace;
    std::chrono::system_clock::duration duration;
};


struct LeftHeavy 
{
    std::vector<CountedTrace> traces;
    int depth;

    static LeftHeavy fromCollapse( const Collapse& );
};


}   //  ::glimmer