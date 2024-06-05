#include "glimmer/guard.h"
#include "glimmer/frame.h"

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
Guard::Guard(
    const std::reference_wrapper<Frame>& frame_,
    const std::string& name,
    const std::source_location source ) : frame( frame_ )
{
    scopeId = frame.get().push( name, source );
}


////////////////////////////////////////////////////////////////////////////////
Guard::~Guard() {
    frame.get().pop( scopeId );
}


}   //  ::glimmer