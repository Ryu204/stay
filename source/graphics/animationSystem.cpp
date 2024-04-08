#include "stay/graphics/animationSystem.hpp"

#include "stay/graphics/render.hpp"
#include "stay/graphics/animation.hpp"

namespace stay
{
    AnimationSystem::AnimationSystem(ecs::Manager* manager)
        : ecs::UpdateSystem{10}
        , ecs::StartSystem{0}
        , ecs::InitSystem{0}
        , ecs::System{manager}
    { }

    void AnimationSystem::start() 
    {
        for (const auto& [e, anim, rd] : mManager->getRegistryRef().view<Animation, Render>().each())
        {
            initialize(anim);
        }
    }

    void AnimationSystem::init(ecs::SystemContext& context)
    {
        mTextures = &context.textures;
    }

    void AnimationSystem::update(float dt) 
    {
        setIndices(dt);
        setTextureRects();
    }

    void AnimationSystem::setIndices(float dt) 
    {
        for (const auto [e, anim] : mManager->getRegistryRef().view<Animation>().each())
        {
            if (!anim.initialized)
                initialize(anim);
            if (anim.action.empty()) 
            {
                anim.setIndex(0);
                continue;
            }
            const auto& action = anim.data.actions.at(anim.action);
            const auto frameTime = 1.F / action.fps;
            anim.elapsedTime += dt;
            if (anim.elapsedTime > frameTime)
            {
                if (action.loop) 
                {
                    if (anim.index + 1 <= action.end)
                        anim.setIndex(anim.index + 1);
                    else
                        anim.setIndex(action.begin);
                }
                else
                    anim.setIndex(std::min(anim.index + 1, action.end));
            }
        }
    }

    void AnimationSystem::setTextureRects()
    {
        for (const auto [e, anim, rd] : mManager->getRegistryRef().view<Animation, Render>().each())
        {
            if (!anim.initialized || !anim.indexChanged)
                continue;
            anim.indexChanged = false;
            const auto row = anim.index / anim.frameSize.x;
            const auto col = anim.index % anim.frameSize.x;
            const Rect rect{
                Vector2{col * anim.frameSize.x, row * anim.frameSize.y},
                Vector2{(col + 1) * anim.frameSize.x, (row + 1) * anim.frameSize.y},
            };
            assert(rd.textureInfo.has_value() && "cannot animate plain color");
            rd.textureInfo->rect = rect;
        }
    }

    void AnimationSystem::initialize(Animation& anim)
    {
        const auto& rd = anim.getNode()->getComponent<Render>();
        assert(rd.textureInfo.has_value() && "cannot animate plain color");
        const auto& texture = mTextures->get(rd.textureInfo->id);
        Vector2Int wholeSize = Vector2Int::from(texture.getSfmlTexture().getSize());
        assert(anim.data.dimension.x != 0 && anim.data.dimension.y != 0 && "probably did not serialize or set dimension");
        assert(wholeSize.x % anim.data.dimension.x == 0 && wholeSize.y % anim.data.dimension.y == 0 && "cannot cut to `dimension`");
        anim.frameSize.x = wholeSize.x / anim.data.dimension.x;
        anim.frameSize.y = wholeSize.y / anim.data.dimension.y;
        anim.initialized = true;
    }
} // namespace stay
