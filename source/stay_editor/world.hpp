#pragma once

#include <memory>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include <stay/world/scene.hpp>

namespace stay::editor
{
    class World : public sf::Drawable
    {
        public:
            World(sf::RenderWindow& window);
            void open(std::filesystem::path&& filename);
            void input(const sf::Event& event);
            void update(sf::Time dt);
            bool isOpen() const;
            void close();
        protected:
            void draw(sf::RenderTarget& target, sf::RenderStates /* states */) const override;
        private:
            std::unique_ptr<stay::Scene> mScene;
            sf::RenderWindow& mWindow;
    };
}