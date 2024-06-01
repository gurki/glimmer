#include "chronos/guard.h"
#include "chronos/profile.h"

namespace chronos {


////////////////////////////////////////////////////////////////////////////////
Guard::Guard( const std::source_location source ) {
    Profile::instance().beginScope( source );
}


////////////////////////////////////////////////////////////////////////////////
Guard::~Guard() {
    Profile::instance().endScope();
}


}   //  ::chronos