# FishClock

FishClock is a [Nexus](https://raidcore.gg/Nexus) addon for **Guild Wars 2** that shows the current day, night, dawn, or dusk state for selected fishing regions in Tyria and Cantha.

It is useful for players working on fishing collections and achievements, especially anyone chasing the title **Cod Swimming Amongst Mere Minnows**.

## Features

- Shows the selected map/region, current period, and time remaining until the next transition.
- Supports Tyria and Cantha fishing time cycles.
- Lets the player choose the region from the Nexus addon settings.
- Displays a small draggable overlay with a fish button.
- Clicking the fish button hides or shows the timer text.

## Installation

1. Download `fish_clock.dll` from the latest release.
2. Place it in your Guild Wars 2 Nexus addons folder.
3. Load FishClock from Nexus.

## Usage

1. Open the FishClock settings in Nexus.
2. Choose the region in `Map to monitor`.
3. Hold `Ctrl` and drag the overlay to reposition it.
4. Click the fish icon to hide or show the timer text.

## Build

Clone the repository with submodules:

```powershell
git clone --recursive https://github.com/gustavotoigo-max/gw2-fishclock.git
```

Or initialize submodules after cloning:

```powershell
git submodule update --init --recursive
```

Build a release DLL:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Configuration Release
```

The DLL is generated at:

```text
build/msvc-release/bin/Release/fish_clock.dll
```

## Version

Current Nexus addon version: `1.0.1.0`.
