#pragma once

#include <string>
#include <source_location>

namespace glimmer {

class Frame;


struct Guard
{
    Guard(
        const std::reference_wrapper<Frame>& frame,
        const std::string& name = {},
        const std::source_location source = std::source_location::current()
    );

    ~Guard();

    const std::reference_wrapper<Frame> frame;
    size_t scopeId;
};


}   //  ::chronos