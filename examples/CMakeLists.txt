cmake_minimum_required( VERSION 3.28 )

project( glimmer-examples
    VERSION 1.0.0
    DESCRIPTION "glimmer examples"
    LANGUAGES CXX
)

add_executable( ${PROJECT_NAME}
    simple.cpp
)

target_link_libraries( ${PROJECT_NAME} 
    PUBLIC
        glimmer
)