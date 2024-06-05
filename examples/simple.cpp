#include <glimmer/glimmer.h>

#include <print>
#include <chrono>
#include <future>

using namespace std::chrono_literals;


struct Object
{
    Object() { GLIMMER_GUARD; }
    ~Object() { GLIMMER_GUARD; }

    float exec() {
        GLIMMER_GUARD;
        std::this_thread::sleep_for( 1ms );
        return 1.f;
    }
};

void workerA() {
    GLIMMER_GUARD;
    std::this_thread::sleep_for( 1ms );
}

void workerB() {
    GLIMMER_NGUARD( "second worker" );
    std::this_thread::sleep_for( 1ms );
}

void workerC() {
    GLIMMER_GUARD;
    workerA();
    std::this_thread::sleep_for( 1ms );
}

void workerD() {
    GLIMMER_NGUARD( "another named guard" );
    Object obj;
    obj.exec();
    std::this_thread::sleep_for( 1ms );
}

void workerE() {
    GLIMMER_GUARD;
    workerC();
    workerD();
    std::this_thread::sleep_for( 1ms );
}


int main( int argc, char* argv[] )
{
    GLIMMER_BEGIN;

    GLIMMER_NBEGIN( "local-scope" );
    std::this_thread::sleep_for( 1ms );
    GLIMMER_END;

    workerA();
    workerC();
    workerD();
    workerE();

    std::async( workerB ).wait();
    std::vector< std::future<void> > futures;

    std::async( [](){ GLIMMER_GUARD; workerA(); } ).wait();

    int val;
    const auto fn = []( const float x, const int& y ) -> double { GLIMMER_GUARD; workerA(); return -1.0; };
    std::async( [&](){ GLIMMER_GUARD; fn( 0, val ); } ).wait();

    for ( int i = 0; i < 10; i++ ) {
        auto futureA = std::async( workerA );
        futures.emplace_back( std::move( futureA ) );
    }

    for ( int i = 0; i < 5; i++ ) {
        workerB();
    }

    for ( auto& f : futures ) {
        f.wait();
    }

    GLIMMER_END;
    glimmer::dump( GLIMMER, "collapsed.txt" );
}