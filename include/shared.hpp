#pragma once

#include "Nexus.h"

namespace addon
{
    inline constexpr uint32_t Signature = 0x47573254; // "GW2T"; change for each real addon.
    inline constexpr const char* Name = "GW2 Nexus Addon Template";
    inline constexpr const char* LogChannel = "GW2AddonTemplate";
    inline constexpr const char* Author = "Nahar.5349";
    inline constexpr const char* Description = "Minimal Nexus addon template for Guild Wars 2.";
    inline constexpr const char* UpdateLink = "";

    extern AddonAPI_t* Api;

    void Log(ELogLevel level, const char* message);
}
