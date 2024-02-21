#include "stay/graphics/renderSystem.hpp"

#include "stay/graphics/render.hpp"
#include "stay/utility/convert.hpp"

namespace stay
{
    namespace sys
    {
        RenderSystem::RenderSystem(ecs::Manager* manager)
            : ecs::RenderSystem{0}
            , ecs::InitSystem{0}
            , ecs::ConfigurableSystem{0}
            , ecs::System{manager}
            , mTextures{nullptr}
            , mBuffer{sf::PrimitiveType::Quads}
        { }

        void RenderSystem::init(ecs::SystemContext& context)
        {
            mTextures = &context.textures;
        }

        void RenderSystem::render(RTarget* target, Node* root)
        {
            traverse(root, target);
            drawObjects(target);
        }

        bool RenderSystem::loadConfig(const Serializable::Data& data) 
        {
            const auto hasTexturesField = data.contains("textures") && data["textures"].is_object();
            if (!data.is_object() && hasTexturesField)
                return false;
            for (const auto& i : data["textures"].items()) // NOLINT
            {
                if(!i.value().is_string())
                    return false;
                mTextures->add(i.key(), i.value().get<std::string>()).load();
            }
            return true;
        }

        void RenderSystem::traverse(Node* node, RTarget* target)
        {
            assert(node != nullptr && "call on nullptr");
            mRenderObjects.clear();
            const auto draw = [&target, this](Node* current, const sf::Transform& tf) -> sf::Transform
            {
                const auto currentTf = tf * utils::transTosfTrans(current->localTransform());
                if (current->hasComponent<Render>())
                {
                    const auto& drawable = current->getComponent<Render>();
                    mRenderObjects.emplace_back(detail::ZOrderPack::from(currentTf, drawable));
                }
                return currentTf;
            };
            node->visitChained(draw, sf::Transform::Identity);
        }

        void RenderSystem::drawObjects(RTarget* target)
        {
            std::sort(mRenderObjects.rbegin(), mRenderObjects.rend());
            mBuffer.clear();
            for (auto i = 0; i < mRenderObjects.size(); ++i)
            {
                const auto& object = mRenderObjects[i];
                for (const auto& v : object.vertices)
                    mBuffer.append(v);
                const auto isFinalObject = i == mRenderObjects.size() - 1;
                const auto notSameCallWithNextObject = !isFinalObject && !object.isSameDrawCall(mRenderObjects[i + 1]);
                const auto shouldDraw = isFinalObject || notSameCallWithNextObject;
                if (shouldDraw) 
                {
                    if (object.textureId.has_value())
                    {
                        const auto& textureAsset = mTextures->get(object.textureId.value());
                        target->draw(mBuffer, sf::RenderStates{&textureAsset.getSfmlTexture()});
                    }
                    else
                    {
                        target->draw(mBuffer);
                    }
                    mBuffer.clear();
                }
            }
        }
    } // namespace sys
} // namespace stay
