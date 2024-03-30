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
        mCamera->adaptTo(target);
        checkCameraControls();
        traverse(root, target);
        drawObjects(target);
    }

    void RenderSystem::checkCameraControls() 
    {
        // Return adjusted size and center
        const auto adjustView = [](const Rect& bounds, float maxHeight, Vector2 size, Vector2 center) 
            -> std::tuple<Vector2, Vector2> {
            const auto scaleRatio = std::min<float>({
                maxHeight / size.y,
                bounds.height() / size.y, 
                bounds.width() / size.x
            });
            size = size * scaleRatio;
            const auto halfSize = Vector2{ size / 2.F };
            center.x = std::max(
                bounds.min().x + halfSize.x,
                std::min(center.x, bounds.max().x - halfSize.x)
            );
            center.y = std::max(
                bounds.min().y + halfSize.y,
                std::min(center.y, bounds.max().y - halfSize.y)
            );
            return std::make_tuple(size, center);
        };
        for (const auto& [entity, camController] : mManager->getRegistryRef().view<CameraController>().each())
        {
            auto tf = Node::getNode(entity)->globalTransform();
            auto [size, pos] = adjustView(
                camController.bounds, 
                camController.height,
                Vector2::from(mCamera->getView().getSize()),
                tf.getPosition()
            );
            
            mCamera->getView().setCenter(pos.toVec2<sf::Vector2f>());
            mCamera->getView().setSize(size.toVec2<sf::Vector2f>());
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
