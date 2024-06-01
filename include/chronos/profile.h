#pragma once

#include <mutex>
#include <unordered_map>
#include <source_location>

namespace chronos {


class Scope;


struct Page
{
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    std::vector<Scope> scopes;
    int level;
};


class Profile 
{
    public:

        void beginScope( const std::source_location source = std::source_location::current() );
        void endScope();

        const auto& pages() const { return pages_; }

        static Profile& instance();

    private:

        std::mutex mutex_;
        std::unordered_map<std::thread::id, Page> pages_;

        static std::unique_ptr<Profile> instance_;
};


}   //  ::chronos