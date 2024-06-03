#include <glimmer/glimmer.h>

#include <print>
#include <chrono>
#include <future>

using namespace std::chrono_literals;


struct Functor
{
    float exec() {
        GLIMMER_GUARD;
        std::this_thread::sleep_for( 2ms );
        return 1.f;
    }
};

void workerA() {
    GLIMMER_GUARD;
    std::this_thread::sleep_for( 10ms );
}

void workerB() {
    GLIMMER_GUARD;
    std::this_thread::sleep_for( 1ms );
}

void workerC() {
    GLIMMER_GUARD;
    workerA();
    std::this_thread::sleep_for( 2ms );
}

void workerD() {
    GLIMMER_GUARD;
    Functor fn;
    fn.exec();
    std::this_thread::sleep_for( 2ms );
}

void workerE() {
    GLIMMER_GUARD;
    workerC();
    workerD();
    std::this_thread::sleep_for( 10ms );
}


int main( int argc, char* argv[] )
{
    GLIMMER_BEGIN;

    workerA();
    workerC();
    workerD();
    workerE();

    std::async( workerB ).wait();
    std::vector< std::future<void> > futures;

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
    glimmer::dumpStackCollapse( GLIMMER, "out.txt" );
}