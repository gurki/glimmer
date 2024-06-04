#pragma once

#include <mutex>
#include <unordered_map>
#include <source_location>

namespace glimmer {


class Scope;


class Frame 
{
    public:

        Frame();
        
        void push( 
            const std::string& name = {}, 
            const std::source_location source = std::source_location::current() 
        );
        void pop();
        void setThreadName( const std::string& );

        const auto& start() const { return start_; }    //  construction
        const auto& end() const { return end_; }        //  last scope end
        const auto& scopes() const { return scopes_; }
        const auto& names() const { return names_; }
        
        bool empty() const { return start_ == end_; }

        static Frame& instance();

    private:

        std::chrono::system_clock::time_point start_;
        std::chrono::system_clock::time_point end_;

        //  thread-protected
        std::mutex mutex_;
        std::unordered_map<std::thread::id, int> levels_;
        std::unordered_map<std::thread::id, std::string> names_;
        std::vector<Scope> scopes_;
        
        //  global
        static std::unique_ptr<Frame> instance_;
};


}   //  ::glimmer