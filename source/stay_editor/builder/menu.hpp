#pragma once

#include <utility>

#include <imgui.h>
#include <imgui_internal.h>

#include "../builderContext.hpp"
#include "../world.hpp"

namespace stay::editor
{
    struct MenuBuilder
    {
        MenuBuilder(BuilderContext context)
            : context{std::move(context)}
        {}
        MenuBuilder& build()
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if(ImGui::BeginMenu("Open"))
                    {
                        openWorldFile();
                        ImGui::EndMenu();
                    }
                    closeWorldFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            return *this;
        }
        void openWorldFile()
        {
            static bool badState = false;
            static std::string log;
            static char buffer[100] = "";
            if (ImGui::InputText("Enter", buffer, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                try { context.world.open(std::filesystem::path{buffer}); }
                catch (std::exception& e)
                {
                    badState = true;
                    log = e.what();
                }
            
            if (badState)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{1.F, 0.F, 0.F, 1.F});
                ImGui::TextWrapped("Error: %s", log.c_str());
                if (ImGui::Button("Okay, I get it"))
                    badState = false;
                ImGui::PopStyleColor(1);
            }
        }
        void closeWorldFile()
        {
            if (!context.world.isOpen())
            {
                ImGui::TextDisabled("Close");
                return;
            }
            if (ImGui::MenuItem("Close"))
                context.world.close();
        }

        BuilderContext context;
    };
}