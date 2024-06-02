#pragma once

#include <source_location>

namespace glimmer {


struct Guard {
    Guard( const std::source_location source = std::source_location::current() );
    ~Guard();
};


}   //  ::chronos