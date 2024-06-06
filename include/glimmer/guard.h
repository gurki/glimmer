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
        const std::stacktrace& trace = std::stacktrace::current()
    );

    ~Guard();

    const std::reference_wrapper<Frame> frame;
    const size_t scopeId;
};


}   //  ::chronos