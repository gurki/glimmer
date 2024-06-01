#include <chronos/chronos.h>

#include <print>
#include <chrono>
#include <future>

using namespace std::chrono_literals;


struct Functor 
{
    float fun() { 
        chronos::Guard guard;
        std::this_thread::sleep_for( 2ms );
        return 1.f; 
    }
};

void workerA() {
    chronos::Guard guard {};
    std::this_thread::sleep_for( 10ms );
}

void workerB() {
    chronos::Guard guard;
    std::this_thread::sleep_for( 1ms );
}

void workerC() {
    chronos::Guard guard {};
    workerA();
    std::this_thread::sleep_for( 2ms );
}

void workerD() {
    chronos::Guard guard;
    Functor fn;
    fn.fun();
    std::this_thread::sleep_for( 2ms );
}

void workerE() {
    chronos::Guard guard;
    workerC();
    workerD();
    std::this_thread::sleep_for( 10ms );
}


int main( int argc, char* argv[] ) 
{
    chronos::Guard guard;

    workerA();
    workerC();
    workerD();
    workerE();

    auto futureA = std::async( workerB );
    futureA.wait();

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

    guard.~Guard();
    chronos::dumpStackCollapse( chronos::Profile::instance(), "out.txt" );
}