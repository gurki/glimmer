#pragma once

#include <string>
#include <source_location>
#include <stacktrace>

namespace glimmer {

class Frame;


struct Guard
{
    Guard(
        const std::reference_wrapper<Frame>& frame,
        const std::string& name = {},
        const std::source_location& source = std::source_location::current(),
        const std::stacktrace& trace = std::stacktrace::current()
    );

    ~Guard();

    const std::reference_wrapper<Frame> frame;
    size_t scopeId;
};


}   //  ::chronos