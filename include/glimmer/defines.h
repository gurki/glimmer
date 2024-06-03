#pragma once

#ifdef GLIMMER_DEFINES
    #define GLIMMER ( glimmer::Profile::instance() )
    #define GLIMMER_BEGIN ( glimmer::Profile::instance().beginScope() )
    #define GLIMMER_NBEGIN(...) ( glimmer::Profile::instance().beginScope(__VA_ARGS__) )
    #define GLIMMER_END ( glimmer::Profile::instance().endScope() )
    #define GLIMMER_GUARD glimmer::Guard _glimmer_guard {}
    #define GLIMMER_NGUARD(...) glimmer::Guard _glimmer_guard(__VA_ARGS__)

#else
    #define GLIMMER
    #define GLIMMER_BEGIN
    #define GLIMMER_NBEGIN
    #define GLIMMER_END
    #define GLIMMER_GUARD
    #define GLIMMER_NGUARD
#endif