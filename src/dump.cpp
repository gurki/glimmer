#include "glimmer/dump.h"
#include "glimmer/frame.h"
#include "glimmer/scope.h"
#include "glimmer/collapse.h"

#include <fstream>
#include <ranges>
#include <print>
#include <functional>

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
void dump( 
    const Frame& frame,
    const std::string& filename )
{
    if ( frame.empty() ) {
        return;
    }

    std::string outname = filename;

    if ( outname.empty() ) {
        const auto time = std::chrono::system_clock::now();
        outname = std::format( 
            "{:%Y-%m-%d_%H%M%S_collapsed.txt}", 
            std::chrono::time_point_cast<std::chrono::seconds>( time )
        );
    }

    std::ofstream fout( outname, std::ios::binary );

    if ( ! fout.is_open() ) {
        return;
    }

    const auto folded = Collapse::fromFrame( frame );

    for ( const auto& item : folded.traces )
    {
        const auto duration = item.end - item.start;
        const auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>( duration );

        const std::string traceStr = std::ranges::fold_left( 
            item.trace | std::views::drop( 1 ), 
            std::string{}, 
            []( const std::string& acc, const std::string& curr ) {
                return std::format( "{};{}", acc, curr );
            }
        );

        const std::string line = std::format( 
            "glimmer;{:04x}{} {}\n", 
            std::atoi( item.trace[ 0 ].c_str() ),
            traceStr, 
            durationUs.count() 
        );
        fout.write( line.c_str(), line.size() );
    }

    fout.close();
}


}   //  ::glimmer