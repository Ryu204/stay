#pragma once 

#include "ecs/system.hpp"
#include "utility/typedef.hpp"

namespace stay
{
    class Camera;
    struct DebugSystem 
        : public ecs::UpdateSystem
        , public ecs::InitSystem
        , public ecs::System<DebugSystem>
    {
            DebugSystem(ecs::Manager* manager);
            void init(ecs::SystemContext& context) override;
            void update(float dt) override;
        private:
            Vector2 mousePosition() const;
            Camera* mCamera;
            RWin* mWindow;
    };
} // namespace stay