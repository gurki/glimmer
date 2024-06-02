#pragma once

#ifdef CHRONOS_DEFINES
    #define CHRONOS ( chronos::Profile::instance() )
    #define CHRONOS_BEGIN ( chronos::Profile::instance().beginScope() )
    #define CHRONOS_END ( chronos::Profile::instance().endScope() )
    #define CHRONOS_GUARD chronos::Guard _chronos_guard
#else
    #define CHRONOS
    #define CHRONOS_BEGIN
    #define CHRONOS_END
    #define CHRONOS_GUARD
#endif