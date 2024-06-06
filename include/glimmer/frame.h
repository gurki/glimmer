#pragma once

#include "glimmer/scope.h"

#include <thread>
#include <mutex>
#include <unordered_map>
#include <source_location>
#include <stacktrace>

namespace glimmer {


class Frame
{
    public:

        Frame();

        size_t push(
            const std::string& name = {},
            const std::stacktrace& trace = std::stacktrace::current()
        );
        void pop(); //  close latest open scope of caller thread
        void pop( const size_t id );
        void setThreadName( const std::string& );

        const auto& start() const { return start_; }    //  construction
        const auto& end() const { return end_; }        //  last scope end
        const auto& scopes() const { return scopes_; }
        const auto& threadNames() const { return threadNames_; }

        bool empty() const { return start_ == end_; }

        static Frame& instance();

    private:

        std::chrono::system_clock::time_point start_;
        std::chrono::system_clock::time_point end_;

        //  thread-protected
        std::mutex mutex_;
        std::vector<Scope> scopes_;
        std::unordered_map<std::thread::id, std::string> threadNames_;

        //  global
        static std::unique_ptr<Frame> instance_;
};


}   //  ::glimmer