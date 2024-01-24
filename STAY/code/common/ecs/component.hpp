#pragma once

#include "type/serializable.hpp"
#include "utility/assignable.hpp"
#include "event/event.hpp"
#include "componentLoader.hpp"
#include "componentLoader.impl.hpp" // IWYU pragma: keep

namespace stay
{
    class Node;
    namespace ecs
    {
        class Manager;
        // Every component type must inherit this struct
        class Component : public Serializable, public utils::Assignable<Entity>, private sf::NonCopyable
        {
            public:
                Node* getNode() const;
                ~Component() override ;
                event::Event<> OnRemoval;
        };
    } // namespace ecs
} // namespace stay


// `className` will be used in save file
#define REGISTER_COMPONENT(className) \
    static inline struct SelfRegister final {\
        SelfRegister() {\
            stay::ecs::componentsLoader().registerComponent<className>(#className);\
        }\
    } selfRegister;

// Only a syntatic sugar
// If you want to perform additional operations, use `REGISTER_COMPONENT` with corresponding serde macro
#define COMPONENT(className, ...) \
    REGISTER_COMPONENT(className)\
    SERIALIZE(__VA_ARGS__)