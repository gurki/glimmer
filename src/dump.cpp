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
auto fold( const TimedTrace& trace ) 
{
    const auto duration = trace.end - trace.start;
    const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

    auto callers = trace.callers | std::views::join_with( ';' );

    return std::format( "{} {}\n", 
        std::string( callers.begin(), callers.end() ), 
        durationUs.count() 
    );
};


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

    for ( const auto& trace : collapse.traces ) {
        //  manual write to force LF line endings
        const std::string line = fold( trace );
        fout.write( line.c_str(), line.size() );    
    }

    const auto bytes = (uint64_t)fout.tellp();
    fout.close();

    return bytes;
}


}   //  ::glimmer