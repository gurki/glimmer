#include "glimmer/frame.h"
#include "glimmer/scope.h"

namespace glimmer {


std::unique_ptr<Frame> Frame::instance_ = nullptr;


////////////////////////////////////////////////////////////////////////////////
Frame::Frame() {
    start_  = std::chrono::system_clock::now();
    end_ = start_;
}


////////////////////////////////////////////////////////////////////////////////
Frame& Frame::instance() 
{
    if ( ! instance_ ) {
        instance_ = std::make_unique<Frame>();
    }

    return *instance_;
}


////////////////////////////////////////////////////////////////////////////////
void Frame::push( 
    const std::string& name, 
    const std::source_location source )
{
    const auto timestamp = std::chrono::system_clock::now();
    const auto thread = std::this_thread::get_id();
    
    Scope scope;
    scope.name = name;
    scope.source = source;
    scope.thread = thread;
    scope.start = timestamp;

    std::scoped_lock lock( mutex_ );

    int& level = levels_[ thread ];
    scope.level = level;
    level++;

    scopes_.emplace_back( std::move( scope ) );
}


////////////////////////////////////////////////////////////////////////////////
void Frame::pop()
{
    const auto thread = std::this_thread::get_id();

    std::scoped_lock lock( mutex_ );
    
    int& level = levels_[ thread ];
    level--;

    //  OPTIM:
    //    use std::stack<std::thread::id, int> to keep track of current
    //    top-level scope and access in O(1) instead of linear search.
    //    additionally minimize lock duration.
    
    const auto rng = std::ranges::find_last_if( scopes_, [ thread, level ]( const Scope& scope ) {
        return scope.thread == thread && scope.level == level;
    });

    if ( rng.empty() ) {
        return;
    }

    auto& scope = *rng.begin();
    scope.closed = true;
    scope.end = std::chrono::system_clock::now();

    end_ = std::max( scope.end, end_ );
}


////////////////////////////////////////////////////////////////////////////////
void Frame::setThreadName( const std::string& name ) {
    const auto thread = std::this_thread::get_id();
    std::scoped_lock lock( mutex_ );
    names_[ thread ] = name;
}


}   //  ::glimmer
