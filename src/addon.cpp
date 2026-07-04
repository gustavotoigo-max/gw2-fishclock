#define FISHCLOCK_DIAGNOSTIC_LOAD_ONLY 0
#define FISHCLOCK_DIAGNOSTIC_CALLBACK_ONLY 0
#define FISHCLOCK_DIAGNOSTIC_RENDER_ONLY 0

// Blocos de diagnostico usados apenas durante investigacao de crash.
// Todos ficam desativados na build normal do addon.
#if FISHCLOCK_DIAGNOSTIC_LOAD_ONLY

#include "shared.hpp"

namespace addon
{
    AddonAPI_t* Api = nullptr;

    void Log(ELogLevel level, const char* message)
    {
        if (Api == nullptr || Api->Log == nullptr)
        {
            return;
        }

        Api->Log(level, LogChannel, message);
    }
}

namespace
{
    AddonDefinition_t AddonDefinition{};

    void AddonLoad(AddonAPI_t* api)
    {
        addon::Api = api;
        addon::Log(LOGL_INFO, "diagnostic load-only build loaded");
    }

    void AddonUnload()
    {
        addon::Log(LOGL_INFO, "diagnostic load-only build unloaded");
        addon::Api = nullptr;
    }
}

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDefinition.Signature = addon::Signature;
    AddonDefinition.APIVersion = NEXUS_API_VERSION;
    AddonDefinition.Name = addon::Name;
    AddonDefinition.Version = AddonVersion_t{0, 1, 1, 0};
    AddonDefinition.Author = addon::Author;
    AddonDefinition.Description = "FishClock diagnostic load-only build.";
    AddonDefinition.Load = AddonLoad;
    AddonDefinition.Unload = AddonUnload;
    AddonDefinition.Flags = AF_None;
    AddonDefinition.Provider = UP_None;
    AddonDefinition.UpdateLink = addon::UpdateLink;

    return &AddonDefinition;
}

#elif FISHCLOCK_DIAGNOSTIC_CALLBACK_ONLY

#include "shared.hpp"

namespace addon
{
    AddonAPI_t* Api = nullptr;

    void Log(ELogLevel level, const char* message)
    {
        if (Api == nullptr || Api->Log == nullptr)
        {
            return;
        }

        Api->Log(level, LogChannel, message);
    }
}

namespace
{
    AddonDefinition_t AddonDefinition{};

    void Render()
    {
    }

    void RenderOptions()
    {
    }

    void AddonLoad(AddonAPI_t* api)
    {
        addon::Api = api;
        addon::Log(LOGL_INFO, "diagnostic callback-only build loading");

        if (addon::Api != nullptr && addon::Api->GUI_Register != nullptr)
        {
            addon::Api->GUI_Register(RT_Render, Render);
            addon::Api->GUI_Register(RT_OptionsRender, RenderOptions);
        }

        addon::Log(LOGL_INFO, "diagnostic callback-only build loaded");
    }

    void AddonUnload()
    {
        if (addon::Api != nullptr && addon::Api->GUI_Deregister != nullptr)
        {
            addon::Api->GUI_Deregister(Render);
            addon::Api->GUI_Deregister(RenderOptions);
        }

        addon::Log(LOGL_INFO, "diagnostic callback-only build unloaded");
        addon::Api = nullptr;
    }
}

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDefinition.Signature = addon::Signature;
    AddonDefinition.APIVersion = NEXUS_API_VERSION;
    AddonDefinition.Name = addon::Name;
    AddonDefinition.Version = AddonVersion_t{0, 1, 2, 1};
    AddonDefinition.Author = addon::Author;
    AddonDefinition.Description = "FishClock diagnostic callback-only build.";
    AddonDefinition.Load = AddonLoad;
    AddonDefinition.Unload = AddonUnload;
    AddonDefinition.Flags = AF_None;
    AddonDefinition.Provider = UP_None;
    AddonDefinition.UpdateLink = addon::UpdateLink;

    return &AddonDefinition;
}

#elif FISHCLOCK_DIAGNOSTIC_RENDER_ONLY

#include "shared.hpp"

#include <imgui.h>

namespace addon
{
    AddonAPI_t* Api = nullptr;

    void Log(ELogLevel level, const char* message)
    {
        if (Api == nullptr || Api->Log == nullptr)
        {
            return;
        }

        Api->Log(level, LogChannel, message);
    }
}

namespace
{
    // Estado compartilhado do overlay e das configuracoes do addon.
    bool IsWindowVisible = true;
    AddonDefinition_t AddonDefinition{};

    void ConfigureImGui()
    {
        if (addon::Api == nullptr || addon::Api->ImguiContext == nullptr)
        {
            return;
        }

        if (addon::Api->ImguiMalloc != nullptr && addon::Api->ImguiFree != nullptr)
        {
            ImGui::SetAllocatorFunctions(
                reinterpret_cast<void* (*)(size_t, void*)>(addon::Api->ImguiMalloc),
                reinterpret_cast<void (*)(void*, void*)>(addon::Api->ImguiFree));
        }

        if (addon::Api->ImguiMalloc != nullptr && addon::Api->ImguiFree != nullptr)
        {
            ImGui::SetAllocatorFunctions(
                reinterpret_cast<void* (*)(size_t, void*)>(addon::Api->ImguiMalloc),
                reinterpret_cast<void (*)(void*, void*)>(addon::Api->ImguiFree));
        }

        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(addon::Api->ImguiContext));
    }

    void Render()
    {
        ConfigureImGui();

        if (!IsWindowVisible)
        {
            return;
        }

        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("FishClock Diagnostic", &IsWindowVisible, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextUnformatted("FishClock render diagnostic loaded.");
        }

        ImGui::End();
    }

    void RenderOptions()
    {
        ConfigureImGui();
        ImGui::Checkbox("Show FishClock diagnostic window", &IsWindowVisible);
    }

    void AddonLoad(AddonAPI_t* api)
    {
        addon::Api = api;
        addon::Log(LOGL_INFO, "diagnostic render-only build loading");

        if (addon::Api != nullptr && addon::Api->GUI_Register != nullptr)
        {
            addon::Api->GUI_Register(RT_Render, Render);
            addon::Api->GUI_Register(RT_OptionsRender, RenderOptions);
        }

        addon::Log(LOGL_INFO, "diagnostic render-only build loaded");
    }

    void AddonUnload()
    {
        if (addon::Api != nullptr && addon::Api->GUI_Deregister != nullptr)
        {
            addon::Api->GUI_Deregister(Render);
            addon::Api->GUI_Deregister(RenderOptions);
        }

        addon::Log(LOGL_INFO, "diagnostic render-only build unloaded");
        addon::Api = nullptr;
    }
}

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDefinition.Signature = addon::Signature;
    AddonDefinition.APIVersion = NEXUS_API_VERSION;
    AddonDefinition.Name = addon::Name;
    AddonDefinition.Version = AddonVersion_t{0, 1, 2, 0};
    AddonDefinition.Author = addon::Author;
    AddonDefinition.Description = "FishClock diagnostic render-only build.";
    AddonDefinition.Load = AddonLoad;
    AddonDefinition.Unload = AddonUnload;
    AddonDefinition.Flags = AF_None;
    AddonDefinition.Provider = UP_None;
    AddonDefinition.UpdateLink = addon::UpdateLink;

    return &AddonDefinition;
}

#else

#include "shared.hpp"
#include "resource.h"

#include <imgui.h>

#include <Windows.h>

#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>

namespace addon
{
    AddonAPI_t* Api = nullptr;

    void Log(ELogLevel level, const char* message)
    {
        if (Api == nullptr || Api->Log == nullptr)
        {
            return;
        }

        Api->Log(level, LogChannel, message);
    }
}

namespace
{
    bool IsWindowVisible = true;
    bool IsDragging = false;
    ImVec2 WindowPosition(300.0f, 200.0f);
    ImVec2 DragOffset(0.0f, 0.0f);
    float UpdateTimer = 0.0f;
    float TextureRetryTimer = 0.0f;
    std::string DisplayText = "Loading FishClock...";
    ImVec4 DisplayColor(1.0f, 1.0f, 1.0f, 1.0f);
    bool IsTextVisible = true;
    int SelectedButtonSizeIndex = 0;
    Texture_t* ToggleTexture = nullptr;
    AddonDefinition_t AddonDefinition{};

    // Regioes suportadas pelo relogio. IsCantha separa os ciclos de Cantha e Tyria.
    enum class Region : int
    {
        SeitungProvinceCantha = 0,
        NewKainengCityCantha,
        EchovaldWildsCantha,
        DragonsEndCantha,
        GyalaDelveCantha,
        DomainOfKournaTyria,
        CoreTyria,
        RuinsOfOrrTyria
    };

    struct RegionInfo
    {
        Region Id;
        const char* Label;
        bool IsCantha;
    };

    struct ButtonSizeOption
    {
        const char* Label;
        float Size;
    };

    constexpr std::array<RegionInfo, 8> Regions{{
        {Region::SeitungProvinceCantha, "Seitung Province (Cantha)", true},
        {Region::NewKainengCityCantha, "New Kaineng City (Cantha)", true},
        {Region::EchovaldWildsCantha, "The Echovald Wilds (Cantha)", true},
        {Region::DragonsEndCantha, "Dragon's End (Cantha)", true},
        {Region::GyalaDelveCantha, "Gyala Delve (Cantha)", true},
        {Region::DomainOfKournaTyria, "Domain of Kourna (Tyria)", false},
        {Region::CoreTyria, "Core Tyria", false},
        {Region::RuinsOfOrrTyria, "Ruins of Orr (Tyria)", false},
    }};

    int SelectedRegionIndex = 0;

    // Tamanhos do botao do peixe. O primeiro preserva o tamanho original do addon.
    constexpr std::array<ButtonSizeOption, 3> ButtonSizes{{
        {"Small", 28.0f},
        {"Medium", 38.0f},
        {"Large", 48.0f},
    }};

    // Nexus fornece o contexto do ImGui. O addon precisa seleciona-lo antes de desenhar.
    void ConfigureImGui()
    {
        if (addon::Api == nullptr || addon::Api->ImguiContext == nullptr)
        {
            addon::Log(LOGL_WARNING, "ImguiContext unavailable");
            return;
        }

        if (addon::Api->ImguiMalloc != nullptr && addon::Api->ImguiFree != nullptr)
        {
            ImGui::SetAllocatorFunctions(
                reinterpret_cast<void* (*)(size_t, void*)>(addon::Api->ImguiMalloc),
                reinterpret_cast<void (*)(void*, void*)>(addon::Api->ImguiFree));
        }

        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(addon::Api->ImguiContext));
    }

    // Recupera o modulo da DLL para carregar recursos embutidos, como o icone do peixe.
    HMODULE GetAddonModule()
    {
        HMODULE module = nullptr;
        if (!GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(&GetAddonModule),
            &module))
        {
            return nullptr;
        }

        return module;
    }

    // Carrega a textura do botao. A API do Nexus pode devolver a textura alguns frames depois.
    void LoadTexture()
    {
        if (addon::Api == nullptr)
        {
            return;
        }

        if (addon::Api->Textures_Get != nullptr)
        {
            ToggleTexture = addon::Api->Textures_Get("FishClock.Toggle");
            if (ToggleTexture != nullptr && ToggleTexture->Resource != nullptr)
            {
                return;
            }
        }

        if (addon::Api->Textures_GetOrCreateFromResource == nullptr)
        {
            return;
        }

        HMODULE module = GetAddonModule();
        if (module == nullptr)
        {
            addon::Log(LOGL_WARNING, "addon module unavailable for texture resource");
            return;
        }

        ToggleTexture = addon::Api->Textures_GetOrCreateFromResource("FishClock.Toggle", IDB_FISHCLOCK_TOGGLE, module);
        if (ToggleTexture == nullptr || ToggleTexture->Resource == nullptr)
        {
            addon::Log(LOGL_DEBUG, "FishClock toggle texture not ready");
        }
    }

    // Garante que o indice salvo em disco sempre aponta para uma regiao valida.
    const RegionInfo& GetSelectedRegion()
    {
        if (SelectedRegionIndex < 0 || SelectedRegionIndex >= static_cast<int>(Regions.size()))
        {
            SelectedRegionIndex = 0;
        }

        return Regions[static_cast<size_t>(SelectedRegionIndex)];
    }

    const ButtonSizeOption& GetSelectedButtonSize()
    {
        if (SelectedButtonSizeIndex < 0 || SelectedButtonSizeIndex >= static_cast<int>(ButtonSizes.size()))
        {
            SelectedButtonSizeIndex = 0;
        }

        return ButtonSizes[static_cast<size_t>(SelectedButtonSizeIndex)];
    }

    // Usa a pasta propria do addon dentro do Nexus para manter settings fora da pasta do jogo.
    std::filesystem::path GetSettingsPath()
    {
        if (addon::Api == nullptr || addon::Api->Paths_GetAddonDirectory == nullptr)
        {
            return {};
        }

        const char* directory = addon::Api->Paths_GetAddonDirectory("FishClock");
        if (directory == nullptr || directory[0] == '\0')
        {
            return {};
        }

        return std::filesystem::path(directory) / "settings.ini";
    }

    // Salva preferencias simples: regiao, posicao, visibilidade e tamanho do botao.
    void SaveSettings()
    {
        const std::filesystem::path path = GetSettingsPath();
        if (path.empty())
        {
            return;
        }

        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error)
        {
            addon::Log(LOGL_WARNING, "failed to create settings directory");
            return;
        }

        std::ofstream file(path, std::ios::trunc);
        if (!file.is_open())
        {
            addon::Log(LOGL_WARNING, "failed to open settings file");
            return;
        }

        file << SelectedRegionIndex << '\n';
        file << WindowPosition.x << ' ' << WindowPosition.y << '\n';
        file << (IsWindowVisible ? 1 : 0) << '\n';
        file << (IsTextVisible ? 1 : 0) << '\n';
        file << SelectedButtonSizeIndex << '\n';
    }

    // Carrega preferencias salvas e aplica valores padrao quando o arquivo ainda nao existe.
    void LoadSettings()
    {
        const std::filesystem::path path = GetSettingsPath();
        if (path.empty())
        {
            return;
        }

        std::ifstream file(path);
        if (!file.is_open())
        {
            return;
        }

        int visible = 1;
        int textVisible = 1;
        file >> SelectedRegionIndex;
        file >> WindowPosition.x >> WindowPosition.y;
        file >> visible;
        file >> textVisible;
        if (!(file >> SelectedButtonSizeIndex))
        {
            SelectedButtonSizeIndex = 0;
        }

        if (SelectedRegionIndex < 0 || SelectedRegionIndex >= static_cast<int>(Regions.size()))
        {
            SelectedRegionIndex = 0;
        }

        if (SelectedButtonSizeIndex < 0 || SelectedButtonSizeIndex >= static_cast<int>(ButtonSizes.size()))
        {
            SelectedButtonSizeIndex = 0;
        }

        IsWindowVisible = visible != 0;
        IsTextVisible = textVisible != 0;
    }

    // O ciclo de dia/noite do GW2 e derivado do horario UTC, sem consultar memoria do jogo.
    std::tm GetUtcNow()
    {
        const auto now = std::chrono::system_clock::now();
        const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

        std::tm utc{};
        gmtime_s(&utc, &nowTime);
        return utc;
    }

    // Converte minutos reais restantes em texto MM:SS para exibir no overlay.
    std::string FormatDuration(double realMinutes)
    {
        int totalSeconds = static_cast<int>(std::ceil(realMinutes * 60.0));
        if (totalSeconds < 0)
        {
            totalSeconds = 0;
        }

        const int minutes = totalSeconds / 60;
        const int seconds = totalSeconds % 60;

        char buffer[16]{};
        snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
        return buffer;
    }

    // Atualiza o periodo atual da regiao selecionada e o tempo ate a proxima troca.
    void UpdateFishClock()
    {
        const RegionInfo& region = GetSelectedRegion();
        const std::tm utc = GetUtcNow();

        const double realMinutesInWindow =
            static_cast<double>((utc.tm_hour % 2) * 60) +
            static_cast<double>(utc.tm_min) +
            (static_cast<double>(utc.tm_sec) / 60.0);

        const double currentInGameMinute = std::fmod(realMinutesInWindow * 12.0, 1440.0);

        const char* state = "UNKNOWN";
        double realMinutesLeft = 0.0;

        // Cantha usa uma janela de noite diferente da janela padrao de Tyria.
        if (region.IsCantha)
        {
            if (currentInGameMinute >= 420.0 && currentInGameMinute < 480.0)
            {
                state = "DAWN";
                realMinutesLeft = (480.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(1.0f, 0.62f, 0.18f, 1.0f);
            }
            else if (currentInGameMinute >= 480.0 && currentInGameMinute < 1140.0)
            {
                state = "DAY";
                realMinutesLeft = (1140.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(1.0f, 0.94f, 0.55f, 1.0f);
            }
            else if (currentInGameMinute >= 1140.0 && currentInGameMinute < 1200.0)
            {
                state = "DUSK";
                realMinutesLeft = (1200.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(0.95f, 0.70f, 0.42f, 1.0f);
            }
            else
            {
                state = "NIGHT";
                const double inGameMinutesUntilDawn =
                    currentInGameMinute >= 1200.0 ? (1440.0 - currentInGameMinute) + 420.0 : 420.0 - currentInGameMinute;
                realMinutesLeft = inGameMinutesUntilDawn / 12.0;
                DisplayColor = ImVec4(0.48f, 0.78f, 1.0f, 1.0f);
            }
        }
        else
        {
            if (currentInGameMinute >= 300.0 && currentInGameMinute < 360.0)
            {
                state = "DAWN";
                realMinutesLeft = (360.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(1.0f, 0.62f, 0.18f, 1.0f);
            }
            else if (currentInGameMinute >= 360.0 && currentInGameMinute < 1200.0)
            {
                state = "DAY";
                realMinutesLeft = (1200.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(1.0f, 0.94f, 0.55f, 1.0f);
            }
            else if (currentInGameMinute >= 1200.0 && currentInGameMinute < 1260.0)
            {
                state = "DUSK";
                realMinutesLeft = (1260.0 - currentInGameMinute) / 12.0;
                DisplayColor = ImVec4(0.95f, 0.70f, 0.42f, 1.0f);
            }
            else
            {
                state = "NIGHT";
                const double inGameMinutesUntilDawn =
                    currentInGameMinute >= 1260.0 ? (1440.0 - currentInGameMinute) + 300.0 : 300.0 - currentInGameMinute;
                realMinutesLeft = inGameMinutesUntilDawn / 12.0;
                DisplayColor = ImVec4(0.48f, 0.78f, 1.0f, 1.0f);
            }
        }

        DisplayText = region.Label;
        DisplayText += ' ';
        DisplayText += state;
        DisplayText += " - Left: ";
        DisplayText += FormatDuration(realMinutesLeft);
    }

    // Desenha a aba de configuracao do addon dentro da janela de opcoes do Nexus.
    void RenderOptions()
    {
        ConfigureImGui();

        ImGui::Checkbox("Show FishClock window", &IsWindowVisible);

        const char* preview = GetSelectedRegion().Label;
        if (ImGui::BeginCombo("Map to monitor", preview))
        {
            for (int index = 0; index < static_cast<int>(Regions.size()); ++index)
            {
                const bool selected = index == SelectedRegionIndex;
                if (ImGui::Selectable(Regions[static_cast<size_t>(index)].Label, selected))
                {
                    SelectedRegionIndex = index;
                    UpdateFishClock();
                    SaveSettings();
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        const char* buttonSizePreview = GetSelectedButtonSize().Label;
        if (ImGui::BeginCombo("Fish button size", buttonSizePreview))
        {
            for (int index = 0; index < static_cast<int>(ButtonSizes.size()); ++index)
            {
                const bool selected = index == SelectedButtonSizeIndex;
                if (ImGui::Selectable(ButtonSizes[static_cast<size_t>(index)].Label, selected))
                {
                    SelectedButtonSizeIndex = index;
                    SaveSettings();
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        ImGui::TextUnformatted("Hold Ctrl and drag the FishClock window to move it.");

        if (ImGui::Button("Reset position"))
        {
            WindowPosition = ImVec2(300.0f, 200.0f);
            SaveSettings();
        }
    }

    // Desenha o overlay transparente com o peixe clicavel e, opcionalmente, o texto do timer.
    void Render()
    {
        ConfigureImGui();

        if (!IsWindowVisible)
        {
            return;
        }

        UpdateTimer -= ImGui::GetIO().DeltaTime;
        if (UpdateTimer <= 0.0f)
        {
            UpdateFishClock();
            UpdateTimer = 1.0f;
        }

        if (ToggleTexture == nullptr || ToggleTexture->Resource == nullptr)
        {
            TextureRetryTimer -= ImGui::GetIO().DeltaTime;
            if (TextureRetryTimer <= 0.0f)
            {
                LoadTexture();
                TextureRetryTimer = 0.5f;
            }
        }

        const bool ctrlDown = ImGui::GetIO().KeyCtrl;

        ImGui::SetNextWindowPos(WindowPosition, ImGuiCond_Always);
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        if (ImGui::Begin("##FishClockOverlay", &IsWindowVisible, flags))
        {
            // Calcula manualmente a linha para manter icone e texto centralizados verticalmente.
            const float selectedButtonSize = GetSelectedButtonSize().Size;
            const ImVec2 buttonSize(selectedButtonSize, selectedButtonSize);
            const float initialX = ImGui::GetCursorPosX();
            const float initialY = ImGui::GetCursorPosY();
            const ImVec2 textSize = IsTextVisible ? ImGui::CalcTextSize(DisplayText.c_str()) : ImVec2(0.0f, 0.0f);
            const float rowHeight = buttonSize.y > textSize.y ? buttonSize.y : textSize.y;
            const float buttonY = initialY + ((rowHeight - buttonSize.y) * 0.5f);
            const float textY = initialY + ((rowHeight - textSize.y) * 0.5f);

            ImGui::SetCursorPos(ImVec2(initialX, buttonY));
            const ImVec2 imageMin = ImGui::GetCursorScreenPos();
            if (ToggleTexture != nullptr && ToggleTexture->Resource != nullptr)
            {
                // InvisibleButton cria a area clicavel enquanto a imagem e desenhada manualmente.
                ImGui::InvisibleButton("##FishClockToggle", buttonSize);
                ImGui::GetWindowDrawList()->AddImage(
                    ToggleTexture->Resource,
                    imageMin,
                    ImVec2(imageMin.x + buttonSize.x, imageMin.y + buttonSize.y),
                    ImVec2(0.0f, 0.0f),
                    ImVec2(1.0f, 1.0f),
                    IM_COL32_WHITE);

                if (ImGui::IsItemClicked())
                {
                    IsTextVisible = !IsTextVisible;
                    SaveSettings();
                }
            }
            else
            {
                ImGui::SetCursorPos(ImVec2(initialX, buttonY));
                if (ImGui::Button("><", buttonSize))
                {
                    IsTextVisible = !IsTextVisible;
                    SaveSettings();
                }
            }

            if (IsTextVisible)
            {
                ImGui::SameLine();
                ImGui::SetCursorPosY(textY);
                ImGui::TextColored(DisplayColor, "%s", DisplayText.c_str());
            }

            const float currentY = ImGui::GetCursorPosY();
            const float rowBottom = initialY + rowHeight;
            if (currentY < rowBottom)
            {
                ImGui::SetCursorPosY(rowBottom);
            }

            const bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

            // Arrastar exige Ctrl para evitar mover o overlay durante cliques normais no jogo.
            if (!IsDragging && hovered && ctrlDown && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                IsDragging = true;
                const ImVec2 mouse = ImGui::GetIO().MousePos;
                DragOffset = ImVec2(mouse.x - WindowPosition.x, mouse.y - WindowPosition.y);
            }

            if (IsDragging)
            {
                if (ctrlDown && ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                    const ImVec2 mouse = ImGui::GetIO().MousePos;
                    WindowPosition = ImVec2(mouse.x - DragOffset.x, mouse.y - DragOffset.y);
                }
                else
                {
                    IsDragging = false;
                    SaveSettings();
                }
            }
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    // Ponto de entrada chamado pelo Nexus quando a DLL e carregada.
    void AddonLoad(AddonAPI_t* api)
    {
        addon::Api = api;
        addon::Log(LOGL_INFO, "loading");

        ConfigureImGui();
        LoadTexture();
        LoadSettings();
        UpdateFishClock();

        if (addon::Api != nullptr && addon::Api->GUI_Register != nullptr)
        {
            addon::Api->GUI_Register(RT_Render, Render);
            addon::Api->GUI_Register(RT_OptionsRender, RenderOptions);
        }

        addon::Log(LOGL_INFO, "loaded");
    }

    // Limpa callbacks e persiste configuracoes quando o addon e descarregado.
    void AddonUnload()
    {
        if (addon::Api != nullptr && addon::Api->GUI_Deregister != nullptr)
        {
            addon::Api->GUI_Deregister(Render);
            addon::Api->GUI_Deregister(RenderOptions);
        }

        SaveSettings();
        IsDragging = false;
        ToggleTexture = nullptr;

        addon::Log(LOGL_INFO, "unloaded");
        addon::Api = nullptr;
    }
}

// Funcao exportada que entrega ao Nexus os metadados e callbacks do addon.
extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDefinition.Signature = addon::Signature;
    AddonDefinition.APIVersion = NEXUS_API_VERSION;
    AddonDefinition.Name = addon::Name;
    AddonDefinition.Version = AddonVersion_t{1, 0, 3, 0};
    AddonDefinition.Author = addon::Author;
    AddonDefinition.Description = addon::Description;
    AddonDefinition.Load = AddonLoad;
    AddonDefinition.Unload = AddonUnload;
    AddonDefinition.Flags = AF_None;
    AddonDefinition.Provider = UP_GitHub;
    AddonDefinition.UpdateLink = addon::UpdateLink;

    return &AddonDefinition;
}

#endif
