#pragma once

#include <SFML/Graphics.hpp>

#include "../../common/ecs/component.hpp"
#include "../../common/type/color.hpp"
#include "../../common/loader/componentLoader.hpp"
#include "../../common/utility/convert.hpp"

namespace stay
{
    namespace comp
    {
        struct Render : public ecs::Component
        {
            Render(const sf::Color& color, const sf::Vector2f& size)
            {
                this->color = color;
                this->size = size;
            }
            sf::Color color;
            sf::Vector2f size;
        };
    } // namespace comp

    template <>
    class ComponentSerializer<comp::Render> : public IComponentSerializer
    {
        public:
            bool deserializeInto(ecs::Manager* manager, ecs::Entity entity, const Json::Value& val) const override
            {
                Vector2 vec;
                Color color;
                bool res = vec.fetch(val["size"]) && color.fetch(val["color"]);
                if (!res)
                    return false;
                manager->addComponents<comp::Render>(entity, sf::Color(color), utils::convertVec2<sf::Vector2f>(vec));
                return true;
            }
            Json::Value serialize(ecs::Manager* manager, ecs::Entity entity) const override
            {
                const auto& data = manager->getComponent<comp::Render>(entity);
                Json::Value res;
                res["size"] = utils::convertVec2<Vector2>(data.size).toJSONObject();
                res["color"] = Color(data.color).toJSONObject();
                return res;
            }
            void createDefault(ecs::Manager* manager, ecs::Entity entity) const
            {
                manager->addComponents<comp::Render>(entity, sf::Color::Magenta, sf::Vector2f());
            }
    };

} // namespace stay