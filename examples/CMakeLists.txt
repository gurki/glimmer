cmake_minimum_required( VERSION 3.28 )

project( glimmer-examples
    VERSION 1.0.0
    DESCRIPTION "glimmer examples"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}-minimal
    minimal.cpp
)

add_executable( ${PROJECT_NAME}-simple
    simple.cpp
)

target_compile_definitions( glimmer
    PUBLIC
        GLIMMER_INCLUDE_RETURN_TYPE
        GLIMMER_INCLUDE_SIGNATURE
        GLIMMER_INCLUDE_SUFFIXES
)

target_link_libraries( ${PROJECT_NAME}-minimal PUBLIC glimmer )
target_link_libraries( ${PROJECT_NAME}-simple PUBLIC glimmer )