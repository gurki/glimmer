cmake_minimum_required( VERSION 3.28 )

project( chronos
    VERSION 0.0.0
    DESCRIPTION "A tiny cpp profiling library"
    LANGUAGES CXX
)

option( BUILD_EXAMPLES "" ON )

add_library( ${PROJECT_NAME} STATIC 
    src/profile.cpp
    src/guard.cpp
    src/dump.cpp
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


if ( BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()