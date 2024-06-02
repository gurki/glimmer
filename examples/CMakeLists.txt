cmake_minimum_required( VERSION 3.28 )

project( chronos-examples
    VERSION 1.0.0
    DESCRIPTION "chronos examples"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}
    simple.cpp
)

target_link_libraries( ${PROJECT_NAME} 
    PUBLIC
        chronos
)