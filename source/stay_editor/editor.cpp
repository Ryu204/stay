#include "stay_editor/editor.hpp"

#include "applicationInfo.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

namespace stay::editor
{
    Editor::Editor()
    {
        initialize();
    }

    void Editor::initialize()
    {
        ApplicationInfo info;
        const std::filesystem::path configFile{"asset/editor/config.json"};
        info.fetch(configFile);
        
        mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 600), "editor");
        if (!ImGui::SFML::Init(*mWindow))
            throw std::runtime_error{"Cannot initialize imgui and SFML"};
        ImGui::GetIO().FontGlobalScale = info.fontScale;
        if (info.darkMode)
            ImGui::StyleColorsDark();
        else
            ImGui::StyleColorsLight();
        mWindow->setFramerateLimit(info.framePerSec);

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    void Editor::run()
    {
        sf::Clock clock;
        while (mWindow->isOpen())
        {
            auto elapsed = clock.restart();
            input(elapsed);
            render();
        }
    }

    void Editor::input(sf::Time dt)
    {
        sf::Event event;
        while (mWindow->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*mWindow, event);
            if (event.type == sf::Event::Closed)
                mWindow->close();
        }
        ImGui::SFML::Update(*mWindow, dt);

        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();
    }

    void Editor::render()
    {
        mWindow->clear();
        ImGui::SFML::Render(*mWindow);
        mWindow->display();
    }
}