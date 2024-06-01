#pragma once

#include <source_location>

namespace chronos {


struct Guard {
    Guard( const std::source_location source = std::source_location::current() );
    ~Guard();
};


}   //  ::chronos