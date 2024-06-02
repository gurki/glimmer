# chronos ⌚
A tiny cpp flame graph profiling library


## Overview
- `Profile` - pages, one per thread
- `Page` - scopes over time
- `Scope` - location and times of entry and exit
- `Guard` - lifetime-based scope


## Output
Dump to textfile in stack collapse format.
Can be used directly with [FlameGraph](https://github.com/brendangregg/FlameGraph) or [speedscope](https://www.speedscope.app/).
Shoutout Brendan Gregg and Jamie Wong for building kool stuff.


## Usage
```cpp
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
```

``` 
# out.txt
7524;main 28179
7524;main;worker 12166
1872;worker 15706
```

![](docs/out.svg)