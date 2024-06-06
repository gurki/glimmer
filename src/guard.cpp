#include "glimmer/guard.h"
#include "glimmer/frame.h"

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
Guard::Guard(
    const std::reference_wrapper<Frame>& frame_,
    const std::string& name,
    const std::stacktrace& trace ) : frame( frame_ )
{
    scopeId = frame.get().push( name, trace );
}


////////////////////////////////////////////////////////////////////////////////
Guard::~Guard() {
    frame.get().pop( scopeId );
}


}   //  ::glimmer