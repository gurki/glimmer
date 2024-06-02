#pragma once

#include <string>

namespace glimmer {


class Profile;

void dumpStackCollapse( 
    const Profile& profile, 
    const std::string& filename 
);


}   //  ::glimmer