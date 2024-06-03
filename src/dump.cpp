#include "glimmer/dump.h"
#include "glimmer/profile.h"
#include "glimmer/scope.h"
#include "glimmer/stackcollapse.h"

#include <fstream>
#include <ranges>

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
void dumpStackCollapse( 
    const Profile& profile,
    const std::string& filename )
{
    std::ofstream fout( filename, std::ios::binary );

    if ( ! fout.is_open() ) {
        return;
    }

    const StackCollapse folded = foldStack( profile );

    for ( const StackTrace& item : folded.traces )
    {
        const auto duration = item.end - item.start;
        const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

        const std::string key = std::ranges::fold_left( 
            item.stack, 
            std::string{}, 
            []( const std::string& acc, const std::string& curr ) {
                return std::format( "{};{}", acc, curr );
            }
        );

        const std::string line = std::format( "{} {}\n", key.substr( 1 ), durationUs.count() );
        fout.write( line.c_str(), line.size() );
    }

    fout.close();
}


}   //  ::glimmer