#include "glimmer/profile.h"
#include "glimmer/scope.h"

namespace glimmer {


std::unique_ptr<Profile> Profile::instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
Profile::Profile() {
    start_  = std::chrono::system_clock::now();
}


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

    int& level = levels_[ thread ];
    scope.level = level;
    level++;

    scopes_.emplace_back( std::move( scope ) );
}


////////////////////////////////////////////////////////////////////////////////
void Profile::endScope()
{
    const auto thread = std::this_thread::get_id();

    std::scoped_lock lock( mutex_ );
    
    int& level = levels_[ thread ];
    level--;

    //  OPTIM: 
    //    use std::stack<std::thread::id, int> to keep track of current 
    //    top-level scope instead of linear search
    
    const auto rng = std::ranges::find_last_if( scopes_, [ thread, level ]( const Scope& scope ) {
        return scope.thread == thread && scope.level == level;
    });

    if ( rng.empty() ) {
        return;
    }

    auto& scope = *rng.begin();
    scope.end = std::chrono::system_clock::now();
    scope.closed = true;
}


}   //  ::glimmer