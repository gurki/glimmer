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
    //  manual begin & end
    GLIMMER_BEGIN;

    //  direct calls
    workerA();
    workerC();
    workerD();
    workerE();

    //  parallel calls
    std::async( workerB ).wait();
    std::async( [](){ GLIMMER_GUARD; workerA(); } ).wait();

    //  complex function signatures
    const auto fn = []( const float x, const int& y ) -> double { 
        GLIMMER_GUARD; 
        workerA(); 
        return -1.0; 
    };

    int val;
    std::async( [&](){ GLIMMER_GUARD; fn( 0, val ); } ).wait();

    //  fully async calls
    std::vector< std::future<void> > futures;

    for ( int i = 0; i < 10; i++ ) {
        auto futureA = std::async( workerA );
        futures.emplace_back( std::move( futureA ) );
    }

    for ( int i = 0; i < 3; i++ ) {
        workerB();
    }

    for ( auto& f : futures ) {
        f.wait();
    }

    GLIMMER_END;

    //  convert and write to disk
    const auto bytes = GLIMMER_NDUMP( "demo.txt" );
    std::println( "wrote {} bytes", bytes.value_or( 0 ) );
}