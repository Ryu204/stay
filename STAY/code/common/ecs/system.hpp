#pragma once

#include "manager.hpp"

namespace stay
{
    namespace ecs
    {
        // Base class for convinience when reference manager
        template <typename T>
        struct System
        {
                System(Manager* manager)
                    : mManager(manager)
                {
                    System<T>::selfRegister.dummy();
                }
                virtual ~System() = default;
                Registry& registry()
                {
                    return mManager->getRegistryRef();
                }            
            protected:
                Manager* mManager;
            private:
                CRTP_UNDERLYING(T)
                static inline struct SelfRegister final
                {
                    SelfRegister()
                    {
                        ecs::manager().registerSystem<T>();
                    }
                    void dummy() {}
                } selfRegister;
        };
    } // namespace ecs
} // namespace stay