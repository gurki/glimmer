#pragma once

#include <mutex>
#include <unordered_map>
#include <source_location>

namespace glimmer {


class Scope;


class Profile 
{
    public:

        Profile();
        
        void beginScope( 
            const std::string& name = {}, 
            const std::source_location source = std::source_location::current() 
        );
        void endScope();

        const auto& start() const { return start_; }
        const auto& scopes() const { return scopes_; }

        static Profile& instance();

    private:

        std::chrono::system_clock::time_point start_;
        std::unordered_map<std::thread::id, int> levels_;
        std::vector<Scope> scopes_;

        std::mutex mutex_;
        
        static std::unique_ptr<Profile> instance_;
};


}   //  ::glimmer