# FishClock

Addon para [Nexus](https://raidcore.gg/Nexus) no **Guild Wars 2**, criado a partir do template `nexus-template`.

## O que vem pronto

- `src/addon.cpp` com `GetAddonDef`, `Load`, `Unload`, render callback e options callback.
- `include/shared.hpp` com metadados do FishClock.
- `CMakeLists.txt` para gerar uma DLL x64.
- `CMakePresets.json` com presets Debug e Release para Visual Studio 2022.
- `build.ps1` para configurar e compilar via PowerShell.
- Submodules para Nexus API e Dear ImGui.

## Preparar o repositorio

Ao clonar este template, traga os submodules:

```powershell
git submodule update --init --recursive
```

Os diretorios `dependencies/nexus-api` e `dependencies/imgui` sao submodules. O repositorio principal deve versionar apenas `.gitmodules` e os gitlinks, sem copiar os arquivos dessas dependencias para dentro do commit.

## Build

Debug:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Configuration Debug
```

Release:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Configuration Release
```

A DLL sera gerada em:

```text
build/msvc-release/bin/Release/fish_clock.dll
```

## Proximos passos

Implementar a logica especifica do FishClock em `src/addon.cpp`.
