#include "glimmer/dump.h"
#include "glimmer/frame.h"
#include "glimmer/scope.h"
#include "glimmer/collapse.h"

#include <fstream>
#include <ranges>
#include <print>
#include <functional>
#include <algorithm>

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
std::expected<uint64_t, std::string> dump( 
    const Frame& frame,
    const std::string& filename )
{
    if ( frame.empty() ) {
        return std::unexpected( "empty frame" );
    }

    std::string outname = filename;

    //  default filename with timestamp prefix
    if ( outname.empty() )
    {
        const auto time = std::chrono::system_clock::now();
    
        outname = std::format( 
            "{:%Y-%m-%d_%H%M%S_collapsed.txt}", 
            std::chrono::time_point_cast<std::chrono::seconds>( time )
        );
    }

    std::ofstream fout( outname, std::ios::binary );

    if ( ! fout.is_open() ) {
        return std::unexpected( std::format( "unable to open file `{}`", outname ) );
    }

    const auto collapse = Collapse::fromFrame( frame );

    for ( const auto& item : collapse.traces )
    {
        const auto duration = item.end - item.start;
        const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

        auto fold = item.trace | std::views::join_with( ';' );

        const std::string line = std::format( "{} {}\n", 
            std::string( fold.begin(), fold.end() ), 
            durationUs.count() 
        );

        fout.write( line.c_str(), line.size() );
    }

    const auto bytes = (uint64_t)fout.tellp();
    fout.close();

    return bytes;
}


}   //  ::glimmer