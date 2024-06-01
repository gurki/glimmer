#pragma once

#include <string>

namespace chronos {


class Profile;

void dumpStackCollapse( 
    const Profile& profile, 
    const std::string& filename 
);


}   //  ::chronos