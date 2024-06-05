#include "glimmer/collapse.h"
#include "glimmer/frame.h"
#include "glimmer/scope.h"

#include <print>
#include <regex>
#include <ranges>

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
Collapse Collapse::fromFrame( const Frame& frame )
{
    const auto& scopes = frame.scopes();
    const auto& names = frame.names();

    Collapse folded {};
    folded.start = frame.start();
    folded.end = frame.end();
    folded.depth = 0;

    std::unordered_map<std::thread::id, std::vector<std::string> > keys;
    std::unordered_map<std::thread::id, int> levels;

    for ( const auto& scope : scopes )
    {
        if ( scope.trace.empty() ) {
            continue;
        }

        static const std::regex reSystem( R"(MSVCP|ntdll|KERNEL32)" );
        static const std::regex reLibrary( R"(Microsoft Visual Studio|vctools)" );

        std::vector<std::string> stack = { std::format( "{}", scope.thread ) };
        auto filtered = (
            scope.trace |
            std::views::filter( []( const std::stacktrace_entry& item ) {
                return ! (
                    std::regex_search( item.description(), reSystem ) ||
                    std::regex_search( item.source_file(), reLibrary )
                );
            }) |
            std::views::reverse
        );

        static const std::regex reName( R"((?:.*!)(.*)\+(0x.+))" );
        // static const std::regex reName( R"((?:.*!)(.*))" );
        auto descriptions = filtered | std::views::transform( &std::stacktrace_entry::description );

        std::ranges::transform( descriptions, std::back_inserter( stack ), []( const std::string& description ) {
            std::smatch match;
            std::regex_match( description, match, reName );
            // return std::format( "{}+{}", match[1].str(), match[2].str() );
            return match[1];
        });

        const std::string key = stack.back();
        folded.traces.emplace_back( key, std::move( stack ), scope.start, scope.end );
        folded.depth = std::max( folded.depth, scope.level );
    }

    return folded;
}


}   //  ::glimmer
