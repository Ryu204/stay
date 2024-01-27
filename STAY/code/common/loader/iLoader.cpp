#include "iLoader.hpp"
#include "graphics/render.hpp"
#include "physics/rigidBody.hpp"

namespace stay
{
    Uptr<Node> ILoader::load(std::filesystem::path&& /* path */)
    {
        auto res = std::make_unique<Node>();
        res->addComponent<Render>(Color{ 200, 100, 45, 255 }, Vector2{ 2, 3 });
        auto& rg = res->addComponent<phys::RigidBody>(Vector2(), 45.F, phys::BodyType::KINEMATIC);
        rg.setAngularVelocity(360.F);
        auto* child = res->createChild();
        child->addComponent<Render>(Color{ 200, 120, 230, 255}, Vector2{4, 4});
        child->localTransform().setPosition(Vector2{0.F, 6.F});
        return std::move(res);
    }

    namespace detail
    {
        ILoader& LoaderHolder::getLoader()
        {
            return *mLoader.get();
        }
        LoaderHolder& loader()
        {
            static LoaderHolder res;
            return res;
        }
    } // namespace detail
} // namespace stay
