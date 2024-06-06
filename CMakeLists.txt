cmake_minimum_required( VERSION 3.28 )

project( glimmer
    VERSION 1.0.0
    DESCRIPTION "A tiny cpp flame graph profiling library"
    LANGUAGES CXX
)

add_library( ${PROJECT_NAME} STATIC
    src/collapse.cpp
    src/dump.cpp
    src/frame.cpp
    src/guard.cpp
)

target_compile_features( ${PROJECT_NAME} PUBLIC
    cxx_std_23
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
)


option( GLIMMER_DISABLE "Activate for zero-impact production builds" OFF )
option( GLIMMER_INCLUDE_FUNCTION_OFFSET "Add function offset suffix, e.g. `main+0x43`" OFF )
option( GLIMMER_INCLUDE_SYSTEM_CALLS "Include system calls" OFF )
option( GLIMMER_INCLUDE_LIBRARY_ENTRIES "Include c/c++ standard library calls" OFF )
option( GLIMMER_BUILD_EXAMPLES "" ON )

if ( GLIMMER_DISABLE )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_DISABLE )
endif()

if( GLIMMER_INCLUDE_FUNCTION_OFFSET )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_FUNCTION_OFFSET )
endif()

if( GLIMMER_INCLUDE_SYSTEM_CALLS )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_SYSTEM_CALLS )
endif()

if( GLIMMER_INCLUDE_LIBRARY_ENTRIES )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_LIBRARY_ENTRIES )
endif()

if ( GLIMMER_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
