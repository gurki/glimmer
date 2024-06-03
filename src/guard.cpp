#include "glimmer/guard.h"
#include "glimmer/profile.h"

namespace glimmer {


////////////////////////////////////////////////////////////////////////////////
Guard::Guard( 
    const std::string& name, 
    const std::source_location source ) 
{
    Profile::instance().beginScope( name, source );
}


////////////////////////////////////////////////////////////////////////////////
Guard::~Guard() {
    Profile::instance().endScope();
}


}   //  ::glimmer