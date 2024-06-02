#pragma once

#ifdef GLIMMER_DEFINES
    #define GLIMMER ( glimmer::Profile::instance() )
    #define GLIMMER_BEGIN ( glimmer::Profile::instance().beginScope() )
    #define GLIMMER_END ( glimmer::Profile::instance().endScope() )
    #define GLIMMER_GUARD glimmer::Guard _glimmer_guard
#else
    #define GLIMMER
    #define GLIMMER_BEGIN
    #define GLIMMER_END
    #define GLIMMER_GUARD
#endif