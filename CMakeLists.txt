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
option( GLIMMER_PREPEND_FUNCTION_TO_NAMED "Prepend current function name to custom named scopes" OFF )
option( GLIMMER_INCLUDE_RETURN_TYPE "Include return type with name, e.g. `int main`" OFF )
option( GLIMMER_INCLUDE_SIGNATURE "Include signature with name, e.g. `main(int,char* [])`" OFF )
option( GLIMMER_INCLUDE_SUFFIXES "Include suffixes with name, e.g. `size() const override`" OFF )
option( GLIMMER_INCLUDE_ALL "Include all of the above e.g. `double compute(float) override`" OFF )

if ( GLIMMER_PREPEND_FUNCTION_TO_NAMED )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_PREPEND_FUNCTION_TO_NAMED )
endif()

if ( GLIMMER_DISABLE )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_DISABLE )
endif()

if( GLIMMER_INCLUDE_RETURN_TYPE )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_RETURN_TYPE )
endif()

if( GLIMMER_INCLUDE_SIGNATURE )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_SIGNATURE )
endif()

if( GLIMMER_INCLUDE_SUFFIXES )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC GLIMMER_INCLUDE_SUFFIXES )
endif()

if( GLIMMER_INCLUDE_ALL )
    target_compile_definitions( ${PROJECT_NAME} PUBLIC
        GLIMMER_INCLUDE_RETURN_TYPE
        GLIMMER_INCLUDE_SIGNATURE
        GLIMMER_INCLUDE_SUFFIXES
    )
endif()


option( GLIMMER_BUILD_EXAMPLES "" ON )

if ( GLIMMER_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
