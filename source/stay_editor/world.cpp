#include "world.hpp"
#include "theme.hpp"

namespace stay::editor
{
    World::World(sf::RenderWindow& window)
        : mWindow{window}
    {}

    void World::update(sf::Time dt)
    {
        if (mScene)
            mScene->update(dt.asSeconds());
    }

    void World::input(const sf::Event& event)
    {
        if (mScene)
            mScene->input(event);
    }

    void World::open(std::filesystem::path&& filename)
    {
        mScene.reset();
        mScene = std::make_unique<Scene>(std::move(filename), mWindow);
        mScene->start();
    }

    bool World::isOpen() const
    {
        return mScene != nullptr;
    }

    void World::close()
    {
        mScene.reset();
    }
    
    void World::draw(sf::RenderTarget& target, sf::RenderStates /* states */ ) const
    {
        target.clear(primaryBackground);
        if (mScene != nullptr)
            mScene->render(target);
    }
}