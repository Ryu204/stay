#pragma once

#include "type/serializable.hpp"
#include "utility/assignable.hpp"
#include "event/event.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        class Manager;
        // Every component type must inherit this struct.
        //
        // Example: `class RigidBody : public Component<RigidBody> {...};`
        //
        // Add `COMPONENT` macro or `COMPONENT_NAME` and `SERIALIZE...` in public field of derive classes
        template <typename T>
        class Component : public Serializable, public utils::Assignable<Entity>, private sf::NonCopyable
        {
            public:
                Node* getNode() const;
                ~Component() override ;
                event::Event<> OnRemoval;
            protected:
                Component() = default;
                static inline struct SelfRegister final {
                    SelfRegister();
                } registerHelper; 
        };
    } // namespace ecs
} // namespace stay

// `name` will be used in save file
#define COMPONENT_NAME(name) \
    static const char* componentName() { return #name;}\
    static void triggerNameRegistration() { registerHelper; }

// Only a syntatic sugar
#define COMPONENT(name, ...) \
    COMPONENT_NAME(name)\
    SERIALIZE(__VA_ARGS__)

