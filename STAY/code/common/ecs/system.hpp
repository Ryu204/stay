#pragma once

#include "manager.hpp"

namespace stay
{
    namespace ecs
    {
        // Base class for convinience when reference manager
        struct System
        {
                System(Manager* manager)
                    : mManager(manager)
                {}
                virtual ~System() = default;
                Registry& registry()
                {
                    return mManager->getRegistryRef();
                }            
            protected:
                Manager* mManager;
        };
    } // namespace ecs
} // namespace stay

#define REGISTER_SYSTEM(className) \
    static inline struct SelfRegister final {\
        SelfRegister() {\
            ecs::manager().registerSystem<className>(#className);\
        }\
    } selfRegister;
