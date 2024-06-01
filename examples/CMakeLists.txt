cmake_minimum_required( VERSION 3.28 )

project( chronos-examples
    VERSION 0.0.0
    DESCRIPTION "A tiny cpp profiling library"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}
    simple.cpp
)

target_link_libraries( ${PROJECT_NAME} 
    PUBLIC
        chronos
)