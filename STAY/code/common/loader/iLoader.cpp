#include "iLoader.hpp"
#include "graphics/render.hpp"
#include "physics/rigidBody.hpp"

namespace stay
{
    void ILoader::load(const std::filesystem::path& /* path */, Node* node)
    {
        node->addComponent<Render>(Color{ 200, 100, 45, 255 }, Vector2{ 2, 3 });
        auto& rg = node->addComponent<phys::RigidBody>(Vector2(), 45.F, phys::BodyType::KINEMATIC);
        rg.setAngularVelocity(360.F);
        auto* child = node->createChild();
        child->addComponent<Render>(Color{ 200, 120, 230, 255}, Vector2{4, 4});
        child->localTransform().setPosition(Vector2{0.F, 6.F});
    }

    void setLoader(std::shared_ptr<ILoader>&& ptr)
    {
        detail::loader().setLoaderPtr(std::move(ptr));
    }

    namespace detail
    {
        LoaderHolder& loader()
        {
            static LoaderHolder res;
            return res;
        }
    } // namespace detail
} // namespace stay
