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
    static const std::regex re( R"((.* )?(?:__cdecl )([\w\d_<>:~]*)(?:::operator \(\))?(\(.*\))( .*)?)" );

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
        const auto thread = scope.thread;
        int& prevLevel = levels[ thread ];

//         std::vector<std::string>& stack = keys[ thread ];

//         if ( stack.empty() )
//         {
//             stack = {
//                 names.contains( thread ) ?
//                 names.at( thread ) :
//                 std::format( "{}", thread )
//             };
//         }

//         const std::string function = scope.source.function_name();

//         std::smatch match;
//         std::string name;
//         std::regex_match( function, match, re );

// #ifdef GLIMMER_INCLUDE_RETURN_TYPE
//         name += match[ 1 ];
// #endif

//         name += match[ 2 ];

// #ifdef GLIMMER_INCLUDE_SIGNATURE
//         name += match[ 3 ];
// #endif

// #ifdef GLIMMER_INCLUDE_SUFFIXES
//         if ( match[ 4 ].length() > 0 ) {
//             name += match[ 4 ].str();
//         }
// #endif

//         //  always remove one level if we didn't go up
//         if ( scope.level <= prevLevel && stack.size() > 1 ) {
//             stack.pop_back();
//         }

//         //  remove an additional layer if we went down
//         if ( scope.level < prevLevel && stack.size() > 1 ) {
//             stack.pop_back();
//         }

// #ifdef GLIMMER_PREPEND_FUNCTION_TO_NAMED
//         std::string key = name;

//         if ( ! scope.name.empty() ) {
//             key += std::format( "::{}", scope.name );
//         }
// #else
//         const std::string key = scope.name.empty() ? name : scope.name;
// #endif

        // stack.push_back( key );

        if ( scope.trace.empty() ) {
            continue;
        }

        const std::string key = scope.trace.crbegin()->description();
        std::vector<std::string> stack( scope.trace.size() + 1 );
        stack[ 0 ] = std::format( "{}", thread );
        std::ranges::transform( scope.trace | std::views::reverse, stack.begin() + 1, &std::stacktrace_entry::description );

        prevLevel = scope.level;

        folded.traces.emplace_back( key, std::move( stack ), scope.start, scope.end );
        folded.depth = std::max( folded.depth, scope.level );
    }

    return folded;
}


}   //  ::glimmer
