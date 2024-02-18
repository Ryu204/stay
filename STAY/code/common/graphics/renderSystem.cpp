#include "renderSystem.hpp"

#include "render.hpp"
#include "utility/convert.hpp"

namespace stay
{
    enum tmpName : std::size_t {
        MOSSY,
    };
    namespace sys
    {
        RenderSystem::RenderSystem(ecs::Manager* manager)
            : ecs::RenderSystem{0}
            , ecs::InitSystem{0}
            , ecs::System{manager}
            , mTextures{nullptr}
            , mBuffer{sf::PrimitiveType::Quads}
        { }

        void RenderSystem::init(ecs::SystemContext& context)
        {
            mTextures = &context.textures;
            mTextures->add(tmpName::MOSSY, "tileset.png").load();
        }

        void RenderSystem::render(RTarget* target, Node* root)
        {
            traverse(root, target);
            drawObjects(target);
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
            std::sort(mRenderObjects.begin(), mRenderObjects.end());
            mBuffer.clear();
            for (auto i = 0; i < mRenderObjects.size(); ++i)
            {
                const auto& object = mRenderObjects[i];
                for (const auto& v : object.vertices)
                    mBuffer.append(v);
                const auto isFinalObject = i == mRenderObjects.size() - 1;
                const auto notSameCallWithLastObject = i > 0 && !object.isSameDrawCall(mRenderObjects[i - 1]);
                const auto shouldDraw = isFinalObject || notSameCallWithLastObject;
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
                }
            }
        }
    } // namespace sys
} // namespace stay
