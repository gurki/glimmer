#include <glimmer/glimmer.h>
#include <future>

void worker() {
    GLIMMER_GUARD;  //  automatic capture of function entry and exit
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
}

int main( int argc, char* argv[] ) 
{
    GLIMMER_BEGIN;  //  manual scope

    worker();       //  sync call
    std::async( worker ).wait();    //  async call   
                    
    GLIMMER_END;    //  automatic close of latest active
    GLIMMER_DUMP;   //  write folded stack collapse to disk
}