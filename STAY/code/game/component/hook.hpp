#pragma once

#include "../../common/ecs/component.hpp"

namespace stay
{
    struct Hook : public ecs::Component
    {
            enum State
            {
                NONE, SHOT, CONNECTED,
            };

            struct Properties : Serializable
            {  
                float speed{5.F};
                float cooldown{1.F};
                float ropeLength{5.F};
                float pullSpeed{5.F};
                SERIALIZE(speed, cooldown, ropeLength, pullSpeed);
            };

            struct Status
            {
                State state{NONE};
                bool shootable{true};
                float timeToCD{0.F};
                float maxLength{0.F};
                Node* bullet;
                std::vector<Node*> createdPins{};
                Node* collisionChecker{nullptr};
            };

            Properties props{};
            Status status{};        
            
            SERIALIZE(props);
    };
} // namespace stay