# GW2 Nexus Addon Template

Template minimo para criar addons do [Nexus](https://raidcore.gg/Nexus) para **Guild Wars 2** usando CMake, Visual Studio 2022, Nexus API e Dear ImGui.

## O que vem pronto

- `src/addon.cpp` com `GetAddonDef`, `Load`, `Unload`, render callback e options callback.
- `include/shared.hpp` com metadados do addon.
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
build/msvc-release/bin/Release/gw2_nexus_addon_template.dll
```

## Criando um addon real

Antes de publicar um addon baseado neste template:

1. Altere `project(...)` e `OUTPUT_NAME` em `CMakeLists.txt`.
2. Altere `Signature`, `Name`, `LogChannel`, `Description` e `UpdateLink` em `include/shared.hpp`.
3. Altere `AddonDefinition.Version` em `src/addon.cpp`.
4. Substitua a UI minima em `Render()` e `RenderOptions()`.

Use uma assinatura unica para cada addon. O valor atual e apenas um placeholder do template.
