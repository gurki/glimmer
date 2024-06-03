#pragma once

#include <source_location>
#include <thread>
#include <chrono>
#include <format>
#include <filesystem>

namespace glimmer {


struct Scope 
{    
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    std::source_location source;
    std::thread::id thread;
    int level = -1;
    bool closed = false;
};


}   //  ::glimmer


template<>
struct std::formatter<glimmer::Scope> 
{
    constexpr auto parse( std::format_parse_context& ctx ) { 
        return ctx.begin(); 
    }

    auto format( const glimmer::Scope& scope, std::format_context& ctx )  const
    {
        const auto dt = scope.end - scope.start;
        const auto durationMs = std::chrono::duration_cast<std::chrono::microseconds>( dt );

        std::string function = scope.source.function_name();

        function = function.substr( 0, function.find_first_of( '(' ) );
        function = function.substr( function.find_last_of( ' ' ) + 1 );
        
        const std::filesystem::path filepath( scope.source.file_name() );
        const std::string filename = filepath.filename().string();

        return std::format_to( 
            ctx.out(),
            "{};{};{}@{}:{}.{} {}",
            scope.thread,
            scope.level,
            function,
            filename,
            scope.source.line(),
            scope.source.column(),
            durationMs.count()
        );
    }
};