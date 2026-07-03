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
    bool IsWindowVisible = true;
    AddonDefinition_t AddonDefinition{};

    void ConfigureImGui()
    {
        if (addon::Api == nullptr || addon::Api->ImguiContext == nullptr)
        {
            addon::Log(LOGL_WARNING, "ImguiContext unavailable");
            return;
        }

        ImGui::SetCurrentContext(static_cast<ImGuiContext*>(addon::Api->ImguiContext));
    }

    void RenderOptions()
    {
        ConfigureImGui();

        ImGui::Checkbox("Show template window", &IsWindowVisible);
    }

    void Render()
    {
        ConfigureImGui();

        if (!IsWindowVisible)
        {
            return;
        }

        if (ImGui::Begin(addon::Name, &IsWindowVisible))
        {
            ImGui::TextUnformatted("Nexus addon template loaded.");
            ImGui::TextUnformatted("Replace this window with the next addon's UI.");
        }

        ImGui::End();
    }

    void AddonLoad(AddonAPI_t* api)
    {
        addon::Api = api;
        addon::Log(LOGL_INFO, "loading");

        ConfigureImGui();

        if (addon::Api != nullptr && addon::Api->GUI_Register != nullptr)
        {
            addon::Api->GUI_Register(RT_Render, Render);
            addon::Api->GUI_Register(RT_OptionsRender, RenderOptions);
        }

        addon::Log(LOGL_INFO, "loaded");
    }

    void AddonUnload()
    {
        if (addon::Api != nullptr && addon::Api->GUI_Deregister != nullptr)
        {
            addon::Api->GUI_Deregister(Render);
            addon::Api->GUI_Deregister(RenderOptions);
        }

        addon::Log(LOGL_INFO, "unloaded");
        addon::Api = nullptr;
    }
}

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDefinition.Signature = addon::Signature;
    AddonDefinition.APIVersion = NEXUS_API_VERSION;
    AddonDefinition.Name = addon::Name;
    AddonDefinition.Version = AddonVersion_t{0, 1, 0, 0};
    AddonDefinition.Author = addon::Author;
    AddonDefinition.Description = addon::Description;
    AddonDefinition.Load = AddonLoad;
    AddonDefinition.Unload = AddonUnload;
    AddonDefinition.Flags = AF_None;
    AddonDefinition.Provider = UP_None;
    AddonDefinition.UpdateLink = addon::UpdateLink;

    return &AddonDefinition;
}
