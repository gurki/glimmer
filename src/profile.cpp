#include "chronos/profile.h"
#include "chronos/scope.h"

namespace chronos {


std::unique_ptr<Profile> Profile::instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
Profile& Profile::instance() 
{
    if ( ! instance_ ) {
        instance_ = std::make_unique<Profile>();
    }

    return *instance_;
}


////////////////////////////////////////////////////////////////////////////////
void Profile::beginScope( const std::source_location source )
{
    const auto thread = std::this_thread::get_id();

    Scope scope;
    scope.source = source;
    scope.thread = thread;
    scope.start = std::chrono::system_clock::now();

    std::scoped_lock lock( mutex_ );

    auto& page = pages_[ thread ];
    scope.level = page.level;
    page.level++;

    page.scopes.emplace_back( std::move( scope ) );
}


////////////////////////////////////////////////////////////////////////////////
void Profile::endScope()
{
    Page* pagePtr = nullptr;
    const auto thread = std::this_thread::get_id();

    {
        std::scoped_lock lock( mutex_ );

        if ( ! pages_.contains( thread ) ) {
            return;
        }

        pagePtr = &pages_.at( thread );
    }

    Page& page = *pagePtr;
    page.level--;
    
    const auto rng = std::ranges::find_last( page.scopes, page.level, []( const Scope& scope ) {
        return scope.level;
    });

    if ( rng.empty() ) {
        return;
    }

    auto& scope = *rng.begin();
    scope.end = std::chrono::system_clock::now();
    scope.closed = true;
}


}   //  ::chronos