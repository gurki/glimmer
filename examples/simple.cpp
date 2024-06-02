#include <chronos/chronos.h>
#include <future>

void worker() {
    CHRONOS_GUARD;
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
}

int main( int argc, char* argv[] ) 
{
    CHRONOS_BEGIN;

    worker();
    std::async( worker ).wait();

    CHRONOS_END;
    chronos::dumpStackCollapse( CHRONOS, "out.txt" );
}