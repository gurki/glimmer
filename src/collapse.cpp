#include "glimmer/collapse.h"
#include "glimmer/frame.h"
#include "glimmer/scope.h"

#include <print>
#include <regex>
#include <ranges>
#include <functional>

namespace glimmer {


static const std::regex reSystem( 
#ifdef _WIN32
    R"((MSVCP|ntdll|KERNEL32))"
#else
    R"((libc|libpthread|libstdc\+\+|libm|libdl|libgcc))" 
#endif
);


static const std::regex reLibrary( 
#ifdef _WIN32
    R"((Microsoft Visual Studio|vctools))" 
#else
    R"((/lib/|/usr/lib/|/opt/|/System/Library/))" 
#endif
);


static const std::regex reFunction( R"((?:.*!)(.*)\+(0x.+))" );


static constexpr auto isSystemEntry = []( const std::stacktrace_entry& item ) -> bool {
    return std::regex_search( item.description(), reSystem );
};


static constexpr auto isLibraryEntry = []( const std::stacktrace_entry& item ) -> bool {
    return std::regex_search( item.source_file(), reLibrary );
};


static constexpr auto parseFunction = []( const std::string& description ) -> std::string {
    std::smatch match;
    std::regex_match( description, match, reFunction );
#ifdef GLIMMER_INCLUDE_FUNCTION_OFFSET
    return std::format( "{}+{}", match[1].str(), match[2].str() );
#else
    return match[1];
#endif
};


static constexpr auto parseTrace = (
#ifndef GLIMMER_INCLUDE_SYSTEM_CALLS
    std::views::filter( std::not_fn( isSystemEntry ) ) |
#endif
#ifndef GLIMMER_INCLUDE_LIBRARY_CALLS
    std::views::filter( std::not_fn( isLibraryEntry ) ) |
#endif 
    std::views::transform( &std::stacktrace_entry::description ) |
    std::views::reverse
);


////////////////////////////////////////////////////////////////////////////////
Collapse Collapse::fromFrame( const Frame& frame )
{ 
    Collapse collapse {};
    collapse.start = frame.start();
    collapse.end = frame.end();
    collapse.depth = 0;

    if ( frame.empty() ) {
        return {};
    }

    for ( const auto& scope : frame.scopes() )
    {
        if ( scope.trace.empty() ) {
            continue;
        }
        
        std::stringstream sstr;
        sstr << scope.thread;
        const uint32_t threadId = std::atoi( sstr.str().c_str() );

        auto descriptions = scope.trace | parseTrace;
        std::vector<std::string> callers = { std::format( "{:04x}", threadId ) };
        std::ranges::transform( descriptions, std::back_inserter( callers ), parseFunction );

        if ( ! scope.name.empty() ) {
            callers.push_back( scope.name );
        }

        auto view = callers | std::views::join_with( ';' );
        collapse.traces.emplace_back( std::move( callers ), scope.start, scope.end );
        collapse.depth = std::max( collapse.depth, (int)callers.size() );
    }

    return collapse;
}


}   //  ::glimmer
