#pragma once

#include <string>
#include <expected>

namespace glimmer {


class Frame;

std::expected<uint64_t, std::string> dump( const Frame&, const std::string& filename = {} );


}   //  ::glimmer