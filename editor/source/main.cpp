#include <iostream>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include "theme.hpp"

int main()
{
    try
    {
        sf::RenderWindow window{sf::VideoMode{500, 600}, "hi"};
        if (!ImGui::SFML::Init(window))
            throw std::runtime_error{"Cannot init windows with imgui"};
        sf::Clock clock;
        while (window.isOpen())
        {
            sf::Event event;
            ImGui::SFML::ProcessEvent(window, event);
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            ImGui::SFML::Update(window, clock.restart());
            ImGui::ShowDemoWindow();
            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();

            window.clear(primaryBackground);
            ImGui::SFML::Render(window);
            window.display();
        }
        ImGui::SFML::Shutdown();
        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown error\n";
        return -2;
    }
}