#include "stay/loader/iLoader.hpp"
#include "stay/graphics/render.hpp"
#include "stay/physics/rigidBody.hpp"

namespace stay
{
    void ILoader::load(std::filesystem::path&& /* path */, Node* root)
    {
        root->addComponent<Render>(Color{ 200, 100, 45, 255 }, Vector2{ 2, 3 });
        auto& rg = root->addComponent<phys::RigidBody>(Vector2(), 45.F, phys::BodyType::KINEMATIC);
        rg.setAngularVelocity(360.F);
        auto* child = root->createChild();
        child->addComponent<Render>(Color{ 200, 120, 230, 255}, Vector2{4, 4});
        child->localTransform().setPosition(Vector2{0.F, 6.F});
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
