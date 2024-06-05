#pragma once

#ifndef GLIMMER_DISABLE
    #define GLIMMER ( glimmer::Frame::instance() )
    #define GLIMMER_BEGIN ( glimmer::Frame::instance().push() )
    #define GLIMMER_NBEGIN(...) ( glimmer::Frame::instance().push(__VA_ARGS__) )
    #define GLIMMER_END ( glimmer::Frame::instance().pop() )
    #define GLIMMER_GUARD glimmer::Guard _glimmer_guard { glimmer::Frame::instance() }
    #define GLIMMER_NGUARD(...) glimmer::Guard _glimmer_guard( glimmer::Frame::instance(), __VA_ARGS__ )
    #define GLIMMER_DUMP glimmer::dump( glimmer::Frame::instance() )
#else
    #define GLIMMER {}
    #define GLIMMER_BEGIN
    #define GLIMMER_NBEGIN
    #define GLIMMER_END
    #define GLIMMER_GUARD
    #define GLIMMER_NGUARD
    #define GLIMMER_DUMP
#endif