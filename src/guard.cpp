#include "glimmer/guard.h"
#include "glimmer/frame.h"

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
Guard::Guard( 
    const std::string& name, 
    const std::source_location source ) 
{
    Frame::instance().push( name, source );
}


////////////////////////////////////////////////////////////////////////////////
Guard::~Guard() {
    Frame::instance().pop();
}


}   //  ::glimmer