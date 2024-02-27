#include "world.hpp"
#include "theme.hpp"

namespace stay::editor
{
    World::World(sf::RenderWindow& window)
        : mWindow{window}
    {}

    void World::open(std::filesystem::path&& filename)
    {
        mScene = std::make_unique<Scene>(std::move(filename), mWindow);
    }
    
    void World::draw(sf::RenderTarget& target, sf::RenderStates /* states */ ) const
    {
        if (mScene != nullptr)
            mScene->render(target);
        else
            target.clear(primaryBackground);
    }
}