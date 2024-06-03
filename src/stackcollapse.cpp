#include "glimmer/stackcollapse.h"
#include "glimmer/profile.h"
#include "glimmer/scope.h"

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
StackCollapse foldStack( const Profile& profile )
{
    StackCollapse folded {};

    folded.start = profile.start();
    const auto& scopes = profile.scopes();
    std::unordered_map<std::thread::id, std::vector<std::string> > keys;
    std::unordered_map<std::thread::id, int> levels;

    for ( const auto& scope : scopes ) 
    {
        const auto thread = scope.thread;
        int& prevLevel = levels[ thread ];

        if ( ! keys.contains( thread ) ) {
            keys[ thread ] = { std::format( "{}", thread ) };
        }

        std::vector<std::string>& stack = keys[ thread ];
        std::string function = scope.source.function_name();
        function = function.substr( 0, function.find_first_of( '(' ) );
        function = function.substr( function.find_last_of( ' ' ) + 1 );

        const auto duration = scope.end - scope.start;
        const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

        //  always remove one level if we didn't go up
        if ( scope.level <= prevLevel && prevLevel > 0 ) {
            stack.pop_back();
        }
            
        //  remove an additional layer if we went down
        if ( scope.level < prevLevel && prevLevel > 0 ) {
            stack.pop_back();
        }

        std::string key = function;
        
        if ( ! scope.name.empty() ) {
            key += std::format( "::{}", scope.name );
        }

        stack.push_back( key );
        folded.traces.emplace_back( stack, scope.start, scope.end );
        prevLevel = scope.level;
    }

    return folded;
}


}   //  ::glimmer