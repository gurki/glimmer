#pragma once

#include <stacktrace>
#include <thread>
#include <chrono>

namespace glimmer {


struct Scope
{
    std::string name = {};
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    std::stacktrace trace;
    std::thread::id thread;
    bool closed = false;
};


}   //  ::glimmer