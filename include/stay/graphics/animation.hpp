#pragma once

#include "stay/ecs/component.hpp"

namespace stay
{
    class AnimationSystem;
    struct Animation : ecs::Component
    {
        struct Action : Serializable 
        {
            Action(std::size_t begin = {}, std::size_t end = {}, float fps = {}, bool loop = {})
            {
                set(begin, end, fps, loop);
            }
            void set(std::size_t begin = {}, std::size_t end = {}, float fps = {}, bool loop = {})
            {
                this->begin = begin;
                this->end = end;
                this->fps = fps;
                this->loop = loop;
            }
            std::size_t begin{};
            std::size_t end{};
            float fps{};
            bool loop{};
            SERIALIZE(begin, end, fps, loop);
        };
        struct Data : Serializable
        {
            Vector2Int dimension{};
            std::unordered_map<std::string, Action> actions{};
            SERIALIZE(dimension, actions);
        } data{};

        void setAction(const std::string& action)
        {
            assert(data.actions.find(action) != data.actions.end());
            this->action = action;
            setIndex(data.actions[action].begin);
        }
        const std::string& getAction() const 
        {
            return action;
        }
        COMPONENT(Animation, data);
        
        private:
            friend class AnimationSystem;
            void setIndex(std::size_t index) 
            {
                this->index = index;
                indexChanged = true;
                elapsedTime = 0.F;
            }
            Vector2Int frameSize{};
            std::size_t index{};
            bool indexChanged{false};
            float elapsedTime{};
            // Empty is uninitialized
            std::string action{};
            bool initialized{false};
    };
} // namespace stay