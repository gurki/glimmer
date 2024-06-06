cmake_minimum_required( VERSION 3.28 )

project( glimmer-examples
    VERSION 1.0.0
    DESCRIPTION "glimmer examples"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}-minimal
    minimal.cpp
)

add_executable( ${PROJECT_NAME}-demo
    demo.cpp
)

target_compile_definitions( glimmer-examples-demo
    PUBLIC
        GLIMMER_INCLUDE_FUNCTION_OFFSET
        GLIMMER_INCLUDE_SYSTEM_CALLS
        GLIMMER_INCLUDE_LIBRARY_ENTRIES
)

target_link_libraries( ${PROJECT_NAME}-minimal PUBLIC glimmer )
target_link_libraries( ${PROJECT_NAME}-demo PUBLIC glimmer )