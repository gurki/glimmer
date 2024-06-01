#include "chronos/dump.h"
#include "chronos/chronos.h"

#include <fstream>
#include <format>

namespace chronos {


////////////////////////////////////////////////////////////////////////////////
void dumpStackCollapse( 
    const Profile& profile,
    const std::string& filename )
{
    std::ofstream fout( filename, std::ios::binary );

    if ( ! fout.is_open() ) {
        return;
    }

    const auto& pages = profile.pages();

    for ( const auto& [ thread, page ] : pages ) 
    {
        int prevLevel = 0;
        std::string key = std::format( "{}", thread );

        for ( const auto& scope : page.scopes ) 
        {
            std::string function = scope.source.function_name();
            function = function.substr( 0, function.find_first_of( '(' ) );
            function = function.substr( function.find_last_of( ' ' ) + 1 );

            const auto duration = scope.end - scope.start;
            const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

            //  always remove one level if we didn't go up
            if ( scope.level <= prevLevel ) {
                key = key.substr( 0, key.rfind( ";" ) );
            }
             
            //  remove an additional layer if we went down
            if ( scope.level < prevLevel ) {
                key = key.substr( 0, key.rfind( ";" ) );
            }

            key += ";" + function;

            const std::string line = std::format( "{} {}\n", key, durationUs.count() );
            fout.write( line.c_str(), line.size() );
            prevLevel = scope.level;
        }
    }

    fout.close();
}


}   //  ::chronos