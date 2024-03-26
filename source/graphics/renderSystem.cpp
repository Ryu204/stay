#include "stay/graphics/renderSystem.hpp"

#include "stay/graphics/render.hpp"
#include "stay/utility/convert.hpp"
#include "stay/graphics/cameraController.hpp"
#include "stay/world/camera.hpp"
#include "stay/utility/sfutils.hpp"

namespace stay
{
    RenderSystem::RenderSystem(ecs::Manager* manager)
        : ecs::RenderSystem{0}
        , ecs::InitSystem{0}
        , ecs::ConfigurableSystem{0}
        , ecs::System{manager}
        , mTextures{nullptr}
        , mCamera{nullptr}
        , mBuffer{sf::PrimitiveType::Quads}
        , mBackground{std::make_unique<sf::RectangleShape>()}
    { }

    void RenderSystem::init(ecs::SystemContext& context)
    {
        mTextures = &context.textures;
        mCamera = &context.camera;
    }

    void RenderSystem::render(RTarget* target, Node* root)
    {
        checkCameraControls();
        traverse(root, target);
        drawObjects(target);
    }

    void RenderSystem::checkCameraControls() 
    {
        for (const auto& [entity, cameraProps] : mManager->getRegistryRef().view<CameraController>().each())
        {
            mCamera->setHeight(cameraProps.height);
            const auto position = Node::getNode(entity)->globalTransform().getPosition();
            mCamera->getView().setCenter(position.x, position.y);
            break;
        }
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
        const auto& bgr = mTextures->get("background");
        mBackground->setTexture(&bgr.getSfmlTexture());
        mBackground->setScale(sf::Vector2f{1, -1});
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
        target->setView(mCamera->getView());

        mBackground->setPosition(mCamera->getView().getCenter());
        const auto bgrSize = mCamera->getView().getSize();
        mBackground->setSize(bgrSize);
        utils::centerSf(*mBackground);
        target->draw(*mBackground);

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
} // namespace stay
