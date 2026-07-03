#pragma once

#include "Nexus.h"

namespace addon
{
    inline constexpr uint32_t Signature = 0xF15C10CC; // FishClock placeholder; keep unique per addon.
    inline constexpr const char* Name = "FishClock";
    inline constexpr const char* LogChannel = "FishClock";
    inline constexpr const char* Author = "Nahar.5349";
    inline constexpr const char* Description = "Displays a draggable fishing day/night timer for selected Tyria regions.";
    inline constexpr const char* UpdateLink = "";

    extern AddonAPI_t* Api;

    void Log(ELogLevel level, const char* message);
}
