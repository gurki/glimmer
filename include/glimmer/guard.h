#pragma once

#include <string>
#include <source_location>

namespace glimmer {


struct Guard 
{
    Guard( 
        const std::string& name = {}, 
        const std::source_location source = std::source_location::current() 
    );

    ~Guard();
};


}   //  ::chronos